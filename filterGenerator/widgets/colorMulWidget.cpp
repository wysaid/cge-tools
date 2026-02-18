/*
* colorMulWidget.cpp
*
*  Created on: 2014-12-12
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "colorMulWidget.h"

ColorMulWindow::ColorMulWindow(MainWindow* mainWindow, QWidget* parent) : MenuDialogCommon(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_mode(CGE::CGEColorMulFilter::mulFLT)
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::Tool);
	m_mainWindow->enableMenu(false);
	setFixedSize(size());
	initWidgets();
	memset(m_vecValues, 0, sizeof(m_vecValues));
	memset(m_matValues, 0, sizeof(m_matValues));
}

ColorMulWindow::~ColorMulWindow()
{
	if(m_filter != nullptr)
	{
		m_mainWindow->removeFilter(m_filter);
	}
	CGE_LOG_INFO("ColorBlendWindow closed!\n");
	m_mainWindow->enableMenu(true);
}

void ColorMulWindow::initWidgets()
{
    m_ui.vecMulRadioBtn->setChecked(true);
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ColorMulWindow::checkFilter()
{
	CGE::CGEColorMulFilter::MulMode mode;
	if(m_ui.matMulRadioBtn->isChecked())
		mode = CGE::CGEColorMulFilter::mulMAT;
	else if(m_ui.vecMulRadioBtn->isChecked())
		mode = CGE::CGEColorMulFilter::mulVEC;
	else mode = CGE::CGEColorMulFilter::mulFLT;

	if(mode == CGE::CGEColorMulFilter::mulFLT)
		return false;

	if(m_filter == nullptr || m_mode != mode)
	{
		m_mode = mode;
		if(m_filter)
			m_mainWindow->removeFilter(m_filter);
		m_filter = new CGE::CGEColorMulFilter;
		if(!m_filter->initWithMode(m_mode))
		{
			delete m_filter;
			m_filter = nullptr;
			return false;
		}
		m_mainWindow->appendFilter(m_filter);
	}

	return !!m_filter;
}

bool ColorMulWindow::_previewVec(FormatContent* pContent)
{
	if(m_ui.vec0Edit->text().isEmpty() && m_ui.vec1Edit->text().isEmpty() && m_ui.vec2Edit->text().isEmpty())
	{
		return false;
	}

	m_vecValues[0] = m_ui.vec0Edit->text().toFloat();
	m_vecValues[1] = m_ui.vec1Edit->text().toFloat();
	m_vecValues[2] = m_ui.vec2Edit->text().toFloat();

	m_filter->setVEC(m_vecValues[0], m_vecValues[1], m_vecValues[2]);

	if(pContent)
	{
		pContent->cge = QStringLiteral("colormul vec %1 %2 %3").arg(m_vecValues[0]).arg(m_vecValues[1]).arg(m_vecValues[2]);
		m_ui.vec0Edit->setText("");
		m_ui.vec1Edit->setText("");
		m_ui.vec2Edit->setText("");
	}
	return true;
}

bool ColorMulWindow::_previewMat(FormatContent* pContent)
{
	QLineEdit* edits[9] = {
		 m_ui.mat0Edit, m_ui.mat1Edit, m_ui.mat2Edit,
		m_ui.mat3Edit, m_ui.mat4Edit, m_ui.mat5Edit,
		m_ui.mat6Edit, m_ui.mat7Edit, m_ui.mat8Edit
	};

	if(pContent)
		pContent->cge = "colormul mat";

	bool isOK = false;

	for(int i = 0; i != 9; ++i)
	{
		m_matValues[i] = edits[i]->text().toFloat();

		if(pContent)
			pContent->cge += QStringLiteral(" %1").arg(m_matValues[i]);

		if(!edits[i]->text().isEmpty())
			isOK = true;
	}

	if(!isOK)
		return false;

	m_filter->setMAT(m_matValues);
	
	if(pContent)
	{
		for(int i = 0; i != 9; ++i)
		{
			edits[i]->setText("");
		}
	}

	return true;
}

bool ColorMulWindow::_preview(FormatContent* pContent)
{
	if(!checkFilter())
	{
		return false;
	}

	switch (m_mode)
	{
	case CGE::CGEColorMulFilter::mulVEC:
		return _previewVec(pContent);
	case CGE::CGEColorMulFilter::mulMAT:
		return _previewMat(pContent);
	default:;
	}

	return false;
}

void ColorMulWindow::preview()
{
	if(!_preview(nullptr))
	{
		QMessageBox::warning(this, "Invalid Param!", "Please fill in the parameters and select a category!");
		return ;
	}

	m_mainWindow->runFilters();
}

void ColorMulWindow::apply()
{
	FormatContent content;

	if(!_preview(&content))
	{
		QMessageBox::critical(this, "Invalid Param!", "No category selected or parameter error. Please check and re-enter!");
		return ;
	}

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
}
