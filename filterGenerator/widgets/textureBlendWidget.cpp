/*
* textureBlendWidget.cpp
*
*  Created on: 2014-11-19
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "textureBlendWidget.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>


TextureBlendWindow::TextureBlendWindow(MainWindow* mainWindow, QWidget* parent) : MenuDialogCommon(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_blendMode(CGE_BLEND_MIX), m_texture(0), m_wrapFilter(0), m_mapFunc(Map_None)
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::Tool);
	m_mainWindow->enableMenu(false);
	setFixedSize(size());
	initWidgets();
	
}

bool TextureBlendWindow::checkFilter()
{
	if(m_wrapFilter == nullptr)
	{
		m_wrapFilter = new CGE::CGEMutipleEffectFilter();
		if(!m_wrapFilter->initCustomize())
		{
			delete m_wrapFilter;
			m_wrapFilter = nullptr;
		}
		else
		{
			m_mainWindow->appendFilter(m_wrapFilter);
		}
	}
	return !!m_wrapFilter;
}

void TextureBlendWindow::initWidgets()
{
	m_ui.folderEdit->setText(QDir::currentPath());

	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.opacityEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.opacityEdit));

	for(int blendMode = 0; ; ++blendMode)
	{
		const char* blendName = cgeGetBlendModeName((CGETextureBlendMode)blendMode, true);
		if(blendName == nullptr)
			break;
		m_ui.blendModeBox->addItem(QString::fromLocal8Bit(blendName));
	}
	
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
	connect(m_ui.folderChooseBtn, SIGNAL(clicked()), SLOT(chooseResourceFolder()));
//	connect(m_ui.blendModeBox, SIGNAL(currentIndexChanged(int)), SLOT(setBlendMode(int)));
//	connect(m_ui.mapModeBox, SIGNAL(currentIndexChanged(int)), SLOT(setMapMode(int)));
}

bool TextureBlendWindow::_preview()
{
	if(!checkFilter())
		return false;

	auto&& imageName = m_ui.imageNameEdit->text();
	if(m_texture == 0 || imageName != m_texFileName)
	{
		m_texFileName = imageName;
		if(!loadTexture(m_texFileName))
		{
			QMessageBox::information(this, QStringLiteral("Invalid Image Name!"), "Failed to load texture resource. Please check whether your resource name is filled in correctly!");
			return false;
		}
	}

	if(m_filter == nullptr || m_blendMode != m_ui.blendModeBox->currentIndex() || m_mapFunc != m_ui.mapModeBox->currentIndex())
	{
		m_blendMode = (CGETextureBlendMode)m_ui.blendModeBox->currentIndex();
		m_mapFunc = (MapFunction)m_ui.mapModeBox->currentIndex();
		if(!resetFilter(m_blendMode, m_mapFunc))
		{
			QMessageBox::information(this, QStringLiteral("Invalid Blend Mode!"), "Failed to apply blend mode!");
			return false;
		}
	}

	m_filter->setSamplerID(m_texture, false);
	m_intensity = CGE::CGE_MID(m_ui.opacityEdit->text().toFloat() / 100.0f, 0.0f, 1.0f);
	m_ui.opacityEdit->setText(QStringLiteral("%1").arg(m_intensity * 100.0f));
	m_filter->setIntensity(m_intensity);

	CGE_LOG_INFO("Texture id: %d, intensity: %g\n", m_texture, m_intensity);

	return true;
}

void TextureBlendWindow::preview()
{
	if(_preview())
		m_mainWindow->runFilters();
}

void TextureBlendWindow::apply()
{
	if(!_preview())
		return ;
	auto&& mode = QString(cgeGetBlendModeName(m_blendMode)).toLower();
	auto intensity = int(m_intensity * 100.0f);

	FilterFormats::Format format;

	switch (m_mapFunc)
	{
	case Map_None:
		format = FilterFormats::texBlend;
		break;
	case Map_KeepRatio:
		format = FilterFormats::texBlendKeepRatio;
		break;
	case Map_Tile:
		format = FilterFormats::tileBlend;
		break;
	default:
		break;
	}

	FormatContent content;
	content.cge = format.cge.arg(mode).arg(m_texFileName).arg(intensity);
	content.android = format.android.arg(mode).arg(m_texFileName).arg(intensity);
	content.flash = format.flash.arg(mode).arg(m_texFileName).arg(intensity);

	m_mainWindow->appendStep(m_wrapFilter, content);
	m_filter = nullptr;
	m_wrapFilter = nullptr;
	m_texture = 0;
	m_ui.imageNameEdit->setText("");
	m_mainWindow->runFilters();
	applyAndQuit();
}

void TextureBlendWindow::chooseResourceFolder()
{
	QString&& folderPath = QFileDialog::getExistingDirectory(this, QStringLiteral("Choose A Folder!"), QDir::currentPath());
	if(!folderPath.isEmpty())
	{
		QDir::setCurrent(folderPath);
		m_ui.folderEdit->setText(folderPath);
		CGE_LOG_INFO("Working Dir Set To: %s\n", (const char*)folderPath.toLocal8Bit());
	}
	else
	{
		CGE_LOG_INFO("Cancel 'chooseResourceFolder'\n");
	}
}

void TextureBlendWindow::setBlendMode(int index)
{
	auto blendMode = (CGETextureBlendMode)index;
	const char* blendName = cgeGetBlendModeName(blendMode, true);
	if(blendName != nullptr)
	{
		CGE_LOG_INFO("Blend mode set to %s\n", blendName);
		m_blendMode = blendMode;
	}
	else
	{
		CGE_LOG_ERROR("Unexpected Blend Mode %d\n!", index);
		return ;
	}
}

void TextureBlendWindow::setMapMode(int index)
{
	CGE_LOG_INFO("Map Mode: %d\n", index);
}

bool TextureBlendWindow::resetFilter(CGETextureBlendMode mode, MapFunction func)
{
	if(!checkFilter())
		return false;

	if(!m_wrapFilter->isEmpty())
	{
		if(m_filter)
			m_filter->setSamplerID(0, false);
		m_wrapFilter->clearFilters();
	}

	switch (func)
	{
	case Map_None:
		m_filter = new CGE::CGEBlendWithResourceFilter;
		break;
	case Map_KeepRatio:
		m_filter = new CGE::CGEBlendKeepRatioFilter;
		break;
	case Map_Tile:
		m_filter = new CGE::CGEBlendTileFilter;
		break;
	default:
		CGE_LOG_ERROR("Unexpected Map Function!");
		return false;
	}

	if(!m_filter->initWithMode(m_blendMode))
	{
		delete m_filter;
		m_filter = nullptr;
		CGE_LOG_ERROR("Init Blend Filter Failed!\n");
		return false;
	}

	m_filter->setTexSize(m_texSize.width, m_texSize.height);
	CGE_LOG_INFO("Blend mode is set to: %s, texture size %d x %d\n", cgeGetBlendModeName(mode, true), m_texSize.width, m_texSize.height);

	m_wrapFilter->addFilter(m_filter);
	return true;
}

bool TextureBlendWindow::loadTexture(const QString& filename)
{
	CGE_LOG_INFO("Loading texture %s\n", (const char*)filename.toLocal8Bit());
	static const float sMaxTexSize = 10000000.0f;
	QImage img(filename);
	if(img.width() < 1 || img.height() < 1)
	{
		return false;
	}
	else if(img.width() * img.height() > sMaxTexSize)
	{
		float ratio = sqrtf(sMaxTexSize / (img.width() * img.height()));
		QSize s(img.width() * ratio, img.height() * ratio);
		img = img.scaled(s, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}

    img = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::ColorOnly);

	m_mainWindow->useCanvasContext();
	m_texSize.set(img.width(), img.height());

	if(m_filter != nullptr)
	{
		m_filter->setTexSize(m_texSize.width, m_texSize.height);
	}

	glDeleteTextures(1, &m_texture);
	m_texture = cgeGenTextureWithBuffer(img.bits(), m_texSize.width, m_texSize.height, GL_RGBA, GL_UNSIGNED_BYTE);
	return m_texture != 0;
}