/*
* colorBlendWidget.cpp
*
*  Created on: 2014-11-21
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "colorBlendWidget.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ColorBlendWindow::ColorBlendWindow(MainWindow* mainWindow, QWidget* parent) : MenuDialogCommon(parent), m_mainWindow(mainWindow), m_wrapFilter(nullptr), m_filter(nullptr), m_filterPixelBlend(nullptr), m_filterVignetteBlend(nullptr), m_filterSelfBlend(nullptr), m_texBlendMode(CGE_BLEND_MIX), m_colorBlendMode(ColorBlend_Pixel), m_vigStart(0.0f), m_vigRange(1.0f), m_bVigBlendAlpha(true)
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::Tool);
	m_mainWindow->enableMenu(false);
	setFixedSize(size());
	initWidgets();
}

ColorBlendWindow::~ColorBlendWindow()
{
	if(m_wrapFilter != nullptr)
	{
		m_mainWindow->removeFilter(m_wrapFilter);
	}
	CGE_LOG_INFO("ColorBlendWindow closed!\n");
	m_mainWindow->enableMenu(true);
}

void ColorBlendWindow::initWidgets()
{
	for(int blendMode = 0; ; ++blendMode)
	{
		const char* blendName = cgeGetBlendModeName((CGETextureBlendMode)blendMode, true);
		if(blendName == nullptr)
			break;
		m_ui.blendModeBox->addItem(QString::fromLocal8Bit(blendName));
	}

	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.opacityEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.opacityEdit));
	m_ui.redEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.redEdit));
	m_ui.greenEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.greenEdit));
	m_ui.blueEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.blueEdit));
	m_ui.alphaEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.alphaEdit));

    m_ui.vigRedEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigRedEdit));
    m_ui.vigGreenEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigGreenEdit));
    m_ui.vigBlueEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigBlueEdit));
    m_ui.vigAlphaEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigAlphaEdit));

	m_ui.vigStartEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigStartEdit));
	m_ui.vigRangeEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigRangeEdit));

	m_ui.vigRangeSlider->setValue(100);

	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
	connect(m_ui.vigStartSlider, SIGNAL(valueChanged(int)), SLOT(setVignetteStart(int)));
	connect(m_ui.vigRangeSlider, SIGNAL(valueChanged(int)), SLOT(setVignetteRange(int)));
	connect(m_ui.tabGroupWidget, SIGNAL(currentChanged(int)), SLOT(switchBlendMode(int)));
}

bool ColorBlendWindow::checkFilter()
{
	auto colorBlendMode = (ColorBlendMode)m_ui.tabGroupWidget->currentIndex();
	auto texBlendMode = (CGETextureBlendMode)m_ui.blendModeBox->currentIndex();
	if(m_wrapFilter == nullptr || m_filter == nullptr ||
		colorBlendMode != m_colorBlendMode || texBlendMode != m_texBlendMode ||
		(colorBlendMode == ColorBlend_Vignette && ((bool)m_bVigBlendAlpha ^ (bool)m_ui.alphaGradientBtn->isChecked()))
		)
	{
		if(m_wrapFilter != nullptr)
		{
			m_wrapFilter->clearFilters();
		}
		else
		{
			m_wrapFilter = new CGE::CGEMutipleEffectFilter;
			if(!m_wrapFilter->initCustomize())
			{
				delete m_wrapFilter;
				m_wrapFilter = nullptr;
				return false;
			}
			m_mainWindow->appendFilter(m_wrapFilter);
		}

		m_filter = nullptr;
		m_filterPixelBlend = nullptr;
		m_filterVignetteBlend = nullptr;
		m_filterSelfBlend = nullptr;
		m_colorBlendMode = colorBlendMode;
		m_texBlendMode = texBlendMode;

		switch (m_colorBlendMode)
		{
		case ColorBlend_Pixel:
			m_filterPixelBlend = new CGE::CGEPixblendFilter;
			if(!m_filterPixelBlend->initWithMode(m_texBlendMode))
			{
				delete m_filterPixelBlend;
                m_filterPixelBlend = nullptr;
				return false;
			}
			m_filter = m_filterPixelBlend;
			break;
		case ColorBlend_Vignette:
			m_bVigBlendAlpha = m_ui.alphaGradientBtn->isChecked();
			if(m_bVigBlendAlpha)
				m_filterVignetteBlend = new CGE::CGEBlendVignette2Filter;
			else
				m_filterVignetteBlend = new CGE::CGEBlendVignette2NoAlphaFilter;
			if(!m_filterVignetteBlend->initWithMode(m_texBlendMode))
			{
                delete m_filterVignetteBlend;
                m_filterVignetteBlend = nullptr;
				return false;
			}
			m_filter = m_filterVignetteBlend;
			break;
		case ColorBlend_Self:
			m_filterSelfBlend = new CGE::CGEBlendWithSelfFilter;
			if(!m_filterSelfBlend->initWithMode(m_texBlendMode))
			{
                delete m_filterSelfBlend;
                m_filterSelfBlend = nullptr;
				return false;
			}
			m_filter = m_filterSelfBlend;
			break;
		default:
			CGE_LOG_ERROR("Unexpected Color Blend Mode!\n");
			return false;
		}
		float intensity = CGE::CGE_MID(m_ui.opacityEdit->text().toFloat() / 100.0f, 0.0f, 1.0f);
		m_filter->setIntensity(intensity);
		m_wrapFilter->addFilter(m_filter);
	}

	return true;
}

bool ColorBlendWindow::_previewBlendPixel()
{
	if(m_filterPixelBlend == nullptr)
	{
		CGE_LOG_ERROR("Error in ColorBlendWindow::_previewBlendPixel");
		return false;
	}

	float r = CGE::CGE_MID(m_ui.redEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);
	float g = CGE::CGE_MID(m_ui.greenEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);
	float b = CGE::CGE_MID(m_ui.blueEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);
	float a = CGE::CGE_MID(m_ui.alphaEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);

	m_filterPixelBlend->setBlendColor(r, g, b, a);
	return true;
}

bool ColorBlendWindow::_previewBlendVignette()
{
	if(m_filterVignetteBlend == nullptr)
	{
		CGE_LOG_ERROR("Error in ColorBlendWindow::_previewBlendVignette");
		return false;
	}

	m_mainWindow->disableRunFilters(true);
	
    float r = CGE::CGE_MID(m_ui.vigRedEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);
    float g = CGE::CGE_MID(m_ui.vigGreenEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);
    float b = CGE::CGE_MID(m_ui.vigBlueEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);
    float a = CGE::CGE_MID(m_ui.vigAlphaEdit->text().toFloat() / 255.0f, 0.0f, 1.0f);

    m_filterVignetteBlend->setBlendColor(r, g, b, a);

    m_vigStart = CGE::CGE_MID(m_ui.vigStartEdit->text().toFloat(), 0.0f, 1.0f);
    m_vigRange = CGE::CGE_MID(m_ui.vigRangeEdit->text().toFloat(), 0.0f, 1.0f);

	m_ui.vigStartSlider->setValue(int(m_vigStart * 100.0f));
	m_ui.vigRangeSlider->setValue(int(m_vigRange * 100.0f));
	m_filterVignetteBlend->setVignette(m_vigStart, m_vigRange);
	m_mainWindow->disableRunFilters(false);

	return true;
}

bool ColorBlendWindow::_previewBlendSelf()
{
	if(m_filterSelfBlend == nullptr)
	{
		CGE_LOG_ERROR("Error in ColorBlendWindow::_previewBlendSelf");
		return false;
	}

	return true;
}

bool ColorBlendWindow::_preview()
{
	if(!checkFilter())
		return false;

	switch (m_colorBlendMode)
	{
	case ColorBlend_Pixel:
		if(!_previewBlendPixel())
			return false;
		break;
	case ColorBlend_Vignette:
		if(!_previewBlendVignette())
			return false;
		break;
	case ColorBlend_Self:
		if(!_previewBlendSelf())
			return false;
		break;
	default:
		break;
	}

	if(m_filter)
	{
		float intensity = CGE::CGE_MID(m_ui.opacityEdit->text().toFloat() / 100.0f, 0.0f, 1.0f);
		m_filter->setIntensity(intensity);
	}

	return true;
}

void ColorBlendWindow::preview()
{
	if(!_preview())
		return ;
	
	m_mainWindow->runFilters();
}

bool ColorBlendWindow::_applyBlendPixel()
{
	if(m_ui.redEdit->text().isEmpty() || m_ui.greenEdit->text().isEmpty() || m_ui.blueEdit->text().isEmpty() || m_ui.alphaEdit->text().isEmpty())
	{
		FILTER_COMMON_ERROR_MSG;
		return false;
	}

	int r = CGE::CGE_MID(m_ui.redEdit->text().toInt(), 0, 255);
	int g = CGE::CGE_MID(m_ui.greenEdit->text().toInt(), 0, 255);
	int b = CGE::CGE_MID(m_ui.blueEdit->text().toInt(), 0, 255);
	int a = CGE::CGE_MID(m_ui.alphaEdit->text().toInt(), 0, 255);
	int intensity = CGE::CGE_MID(m_ui.opacityEdit->text().toInt(), 0, 100);

	auto&& mode = tr(cgeGetBlendModeName(m_texBlendMode)).toLower();

	FormatContent content;
	content.cge = FilterFormats::pixelBlend.cge.arg(mode).arg(r).arg(g).arg(b).arg(a).arg(intensity);
	content.android = FilterFormats::pixelBlend.android.arg(mode).arg(r).arg(g).arg(b).arg(a).arg(intensity);
	content.flash = FilterFormats::pixelBlend.flash.arg(mode).arg(r).arg(g).arg(b).arg(a).arg(intensity);

	m_mainWindow->appendStep(m_wrapFilter, content);
	return true;
}

bool ColorBlendWindow::_applyBlendVignette()
{
	if(m_ui.vigStartEdit->text().isEmpty() || m_ui.vigRangeEdit->text().isEmpty())
	{
QMessageBox::critical(this, "'start' and 'range' cannot be empty!", "Please fill in the values for 'start' and 'range'!", QMessageBox::Ok);
		return false;
	}

	if(m_ui.vigStartEdit->text().toFloat() == 0.0f && m_ui.vigRangeEdit->text().toFloat() == 0.0f)
	{
if(QMessageBox::warning(this, "Refusing stupid effect!", "Warning: If you insist on setting both 'start' and 'range' to 0, please use the standalone color blend in the first option instead. Are you sure you want to proceed with this?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
			return false;
	}

	int r = CGE::CGE_MID(m_ui.vigRedEdit->text().toInt(), 0, 255);
	int g = CGE::CGE_MID(m_ui.vigGreenEdit->text().toInt(), 0, 255);
	int b = CGE::CGE_MID(m_ui.vigBlueEdit->text().toInt(), 0, 255);
	int a = CGE::CGE_MID(m_ui.vigAlphaEdit->text().toInt(), 0, 255);
	int intensity = CGE::CGE_MID(m_ui.opacityEdit->text().toInt(), 0, 100);

	auto&& blendMode = tr(cgeGetBlendModeName(m_texBlendMode)).toLower();

	auto& blendFormatString = m_bVigBlendAlpha ? FilterFormats::vignetteBlend : FilterFormats::vignetteBlendWithNoAlpha;

	FormatContent content;
	content.cge = blendFormatString.cge.arg(blendMode).arg(r).arg(g).arg(b).arg(a).arg(intensity).arg(m_vigStart).arg(m_vigRange);
	content.android = blendFormatString.android.arg(blendMode).arg(r).arg(g).arg(b).arg(a).arg(intensity).arg(m_vigStart).arg(m_vigRange);
	content.flash = blendFormatString.flash.arg(blendMode).arg(r).arg(g).arg(b).arg(a).arg(intensity).arg(m_vigStart).arg(m_vigRange);

	m_mainWindow->appendStep(m_wrapFilter, content);
	return true;
}

bool ColorBlendWindow::_applyBlendSelf()
{
	int intensity = CGE::CGE_MID(m_ui.opacityEdit->text().toInt(), 0, 100);
	auto&& blendMode = tr(cgeGetBlendModeName(m_texBlendMode)).toLower();
	FormatContent content;
	content.cge = FilterFormats::selfBlend.cge.arg(blendMode).arg(intensity);
	content.android = FilterFormats::selfBlend.android.arg(blendMode).arg(intensity);
	content.flash = FilterFormats::selfBlend.flash.arg(blendMode).arg(intensity);

	m_mainWindow->appendStep(m_wrapFilter, content);
	return true;
}

void ColorBlendWindow::apply()
{
	if(!_preview())
		return ;

	switch (m_colorBlendMode)
	{
	case ColorBlend_Pixel:
		if(!_applyBlendPixel())
			return ;
		break;
	case ColorBlend_Vignette:
		if(!_applyBlendVignette())
			return ;
		break;
	case ColorBlend_Self:
		if(!_applyBlendSelf())
			return ;
		break;
	default:
		break;
	}

	m_wrapFilter = nullptr;
	m_filter = nullptr;
	m_filterPixelBlend = nullptr;
	m_filterVignetteBlend = nullptr;
	m_filterSelfBlend = nullptr;
	m_ui.redEdit->setText("");
	m_ui.greenEdit->setText("");
	m_ui.blueEdit->setText("");
	m_ui.alphaEdit->setText("255");
	m_ui.vigRedEdit->setText("");
	m_ui.vigGreenEdit->setText("");
	m_ui.vigBlueEdit->setText("");
	m_ui.vigAlphaEdit->setText("255");
	m_ui.vigStartEdit->setText("");
	m_ui.vigRangeEdit->setText("");
	
	m_mainWindow->runFilters();
	applyAndQuit();
}

void ColorBlendWindow::setVignetteStart(int n)
{
	m_vigStart = n / 100.0f;
	m_ui.vigStartEdit->setText(QStringLiteral("%1").arg(m_vigStart));

	if(m_filterVignetteBlend != nullptr)
	{
		m_filterVignetteBlend->setVignette(m_vigStart, m_vigRange);
	}

	preview();
}

void ColorBlendWindow::setVignetteRange(int n)
{
	m_vigRange = n / 100.0f;
	m_ui.vigRangeEdit->setText(QStringLiteral("%1").arg(m_vigRange));

	if(m_filterVignetteBlend != nullptr)
	{
		m_filterVignetteBlend->setVignette(m_vigStart, m_vigRange);
	}

	preview();
}

void ColorBlendWindow::switchBlendMode(int index)
{
	CGE_LOG_INFO("Switch To Color Blend Mode %d\n", index);
}
