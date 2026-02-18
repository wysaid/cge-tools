/*
* filterRuleTest.cpp
*
*  Created on: 2014-11-26
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "filterruletest.h"
#include "cgeMultipleEffects.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#define BUFFER_SIZE 1024

void* loadBuffer(const char* srcName, void** bufferData, int* w, int* h, CGEBufferFormat* fmt, void* )
{
	CGE_LOG_INFO("Loading resource: %s...\n", srcName);

	QImage* qimg = new QImage(QImage(srcName).convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly));

	QImage& img = *qimg;

	if(img.width() < 1)
	{
		*bufferData = nullptr;
		return nullptr;
	}

	*bufferData = img.bits();
	*w = img.width();
	*h = img.height();
	*fmt = CGE_FORMAT_RGBA_INT8;
	return qimg;
}

bool unloadBuffer(void* arg, void*)
{
	CGE_LOG_INFO("Releasing resouce...");
	delete (QImage*)arg;
	return true;
}


//////////////////////////////////////////////////////////////////////////

FilterRuleTest::FilterRuleTest(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	initWidgets();
	CGE::cgeSetCommonLoadFunction(loadBuffer, nullptr);
	CGE::cgeSetCommonUnloadFunction(unloadBuffer, nullptr);
}

FilterRuleTest::~FilterRuleTest()
{

}

void FilterRuleTest::initWidgets()
{
    m_canvasWidget = new CanvasWidget(m_ui.centralWidget);
    m_ui.canvasWidget->hide();

    m_canvasWidget->setGeometry(m_ui.canvasWidget->geometry());
    m_canvasWidget->show();

	m_ui.resPathEdit->setText(QDir::currentPath());

	connect(m_ui.inputChooseBtn, SIGNAL(clicked()), SLOT(chooseInputImage()));
	connect(m_ui.resChooseBtn, SIGNAL(clicked()), SLOT(chooseResourceFolder()));
	connect(m_ui.testBtn, SIGNAL(clicked()), SLOT(testRule()));
	connect(m_ui.saveBtn, SIGNAL(clicked()), SLOT(saveResult()));
	connect(m_ui.batchBtn, SIGNAL(clicked()), SLOT(batchEffect()));
	connect(m_ui.compBtn, SIGNAL(pressed()), m_canvasWidget, SLOT(setToOriginImage()));
	connect(m_ui.compBtn, SIGNAL(released()), m_canvasWidget, SLOT(setToCurrentImage()));
}

void FilterRuleTest::resizeEvent(QResizeEvent *e)
{
	auto sz = e->size();
    m_ui.ruleEdit->setFixedSize(m_ui.ruleEdit->width(), sz.height() - m_ui.inputBox->height() - m_ui.resBox->height() - m_ui.funcBox->height() - 35);

	auto gEdit = m_ui.ruleEdit->geometry();

    {
        auto g1 = m_ui.inputBox->geometry();		
        m_ui.inputBox->setGeometry(g1.x(), gEdit.height() + 10, g1.width(), g1.height());

		auto g2 = m_ui.resBox->geometry();
		m_ui.resBox->setGeometry(g2.x(), gEdit.height() + g1.height() + 20, g2.width(), g2.height());

		auto g3 = m_ui.funcBox->geometry();
		m_ui.funcBox->setGeometry(g3.x(), sz.height() - g3.height() - 10, g3.width(), g3.height());

    }
	
	{
        auto g = m_ui.canvasWidget->geometry();
        m_ui.canvasWidget->setGeometry(g.x(), g.y(), sz.width() - g.x() - 10, sz.height() - g.y() - 10);
        m_canvasWidget->setGeometry(m_ui.canvasWidget->geometry());
	}
}

void FilterRuleTest::testRule()
{
	using namespace CGE;
	auto&& handler = m_canvasWidget->getHandler();
	auto&& filterRule = m_ui.ruleEdit->toPlainText();
	m_canvasWidget->makeCurrent();
	auto filter = new CGEMutipleEffectFilter;
	if(!filter->initWithEffectString(filterRule.toLocal8Bit()))
	{
		QMessageBox::critical(this, "Failed to initialize filter!", "Invalid rule, or unable to create Filter! See the red text in the console window for details!");
		delete filter;
		return ;
	}
	handler.clearImageFilters(true);
	handler.addImageFilter(filter);
	handler.revertToKeptResult(false);
	handler.processingFilters();

	auto sp = m_canvasWidget->getDisplaySprite();
	auto sz = handler.getOutputFBOSize();
	sp->assignTexture(handler.getTargetTextureID(), sz.width, sz.height);

	m_canvasWidget->update();	
}

void FilterRuleTest::saveResult()
{
	QString&& name = QFileDialog::getSaveFileName(this, QStringLiteral("Save The Image"), QDir::currentPath(), "Images (*.png *.jpg *.xpm)");
	if(name.size() == 0)
	{
		return ;
	}

	if(QFile::exists(name))
	{
		if(QMessageBox::question(this, QStringLiteral("File conflict!"), QString("%1 already exists. Overwrite?").arg(name), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			return ;
	}
	auto& handler = m_canvasWidget->getHandler();
	auto sz = handler.getOutputFBOSize();

	QImage img(sz.width, sz.height, QImage::Format_RGBA8888);
	handler.getOutputBufferData(img.bits(), CGE_FORMAT_RGBA_INT8);
	img.save(name);
}

void FilterRuleTest::chooseInputImage()
{
	QString&& filename = QFileDialog::getOpenFileName(this, QStringLiteral("Select an image file!"), "", QStringLiteral("Images (*.jpg *.png *.gif *.xpm)"));

	if(filename.size() == 0)
	{
		return ;
	}

	if(!m_canvasWidget->openImage(filename))
	{
		CGE_LOG_ERROR("Failed to open image: %s", (const char*)filename.toLocal8Bit());
		return ;
	}

	m_ui.imageImageEdit->setText(filename);
}

void FilterRuleTest::chooseResourceFolder()
{
	QString&& folderPath = QFileDialog::getExistingDirectory(this, QStringLiteral("Choose A Folder!"), QDir::currentPath());
	if(!folderPath.isEmpty())
	{
		QDir::setCurrent(folderPath);
		m_ui.resPathEdit->setText(folderPath);
		CGE_LOG_INFO("Working Dir Set To: %s\n", (const char*)folderPath.toLocal8Bit());
	}
	else
	{
		CGE_LOG_INFO("Cancel 'chooseResourceFolder'");
	}
}

void FilterRuleTest::batchEffect()
{
	CGE_LOG_INFO("Starting batch effect processing. Please select multiple files!\n");

	QStringList&& filenames = QFileDialog::getOpenFileNames(this, "Please select multiple files!", QDir::currentPath());
	if(filenames.empty())
	{
		CGE_LOG_ERROR("Batch processing cancelled!\n");
		return ;
	}

	CGE_LOG_INFO("Please select a location to save the processed files. The output filename will be similar to the source filename.\n");

	QString&& savePath = QFileDialog::getExistingDirectory(this, "Please select a directory to store the output files!");
	if(savePath.isEmpty())
	{
	CGE_LOG_ERROR("Please select a folder to save!");
		return ;
	}
	
	using namespace CGE;

	auto&& filterRule = m_ui.ruleEdit->toPlainText();
	m_canvasWidget->makeCurrent();

	auto filter = new CGEMutipleEffectFilter;
	if(!filter->initWithEffectString(filterRule.toLocal8Bit()))
	{
		QMessageBox::critical(this, "Failed to initialize filter!", "Invalid rule, or unable to create Filter! See the red text in the console window for details!");
		delete filter;
		return ;
	}

	CGEImageHandler handler;
	handler.addImageFilter(filter);

	for(auto& name : filenames)
	{
		QImage img(name);
		if(img.width() < 1 || img.height() < 1)
		{
			CGE_LOG_ERROR("Failed to read image %s...\n", (const char*)name.toLocal8Bit());
			continue;
		}

		CGE_LOG_INFO("Opened image %s successfully, processing...\n", (const char*)name.toLocal8Bit());

		if(img.format() != QImage::Format_RGBA8888)
			img = img.convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly);
		
		m_canvasWidget->makeCurrent();
		if(!handler.updateData(img.bits(), img.width(), img.height(), CGE_FORMAT_RGBA_INT8))
		{
			handler.initWithRawBufferData(img.bits(), img.width(), img.height(), CGE_FORMAT_RGBA_INT8, false);
		}
		handler.processingFilters();
		handler.getOutputBufferData(img.bits(), CGE_FORMAT_RGBA_INT8);
		int index = name.lastIndexOf('/');
		if(index >= 0)
			name.remove(0, index+1);
		QString saveFile = savePath + "/effect_" + name;
		if(!img.save(saveFile))
		{
			CGE_LOG_ERROR("Failed to save image %s...\n", (const char*)saveFile.toLocal8Bit());
		}
	}

	CGE_LOG_INFO("File processing complete!\n");
}