/*
* basicAdjustWidget.cpp
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "basicAdjustWidget.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "toolWidget.h"


enum BasicAdjustEnum
{
	Brightness_Adjust,
	Contrast_Adjust,
	Saturation_Adjust,
	Monochrome_Adjust,
	Exposure_Adjust,
	Hue_Adjust,
	ShadowHighlight_Adjust,
	BlurSharpen_Adjust,
	Vignette_Adjust,
	HSL_Adjust,
	HSV_Adjust,
	WhiteBalance_Adjust,
	ColorBalance_Adjust,
	SelectiveColor_Adjust,
	ColorLevel_Adjust,
	ColorLookup_Adjust,
};

void BrightnessAdjustWidget::initBrightness()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.brightnessEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.brightnessEdit));
	m_ui.brightnessSlider->setRange(-100, 100);
	connect(m_ui.brightnessSlider, SIGNAL(valueChanged(int)), SLOT(setBrightness(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool BrightnessAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createBrightnessFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void BrightnessAdjustWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.brightnessEdit->text().toFloat(), -1.0f, 1.0f);
	m_ui.brightnessSlider->setValue(m_intensity * 100.0f);
	m_filter->setIntensity(m_intensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void BrightnessAdjustWidget::apply()
{
	if(m_ui.brightnessEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::brightness.cge.arg(m_intensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.brightnessEdit->setText("");
}

void BrightnessAdjustWidget::setBrightness(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n / 100.0f;
	m_ui.brightnessEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void ContrastAdjustWidget::initContrast()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.contrastEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.contrastEdit));
	m_ui.contrastSlider->setRange(0, 200);
	m_ui.contrastSlider->setValue(100);
	connect(m_ui.contrastSlider, SIGNAL(valueChanged(int)), SLOT(setContrast(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ContrastAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createContrastFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void ContrastAdjustWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.contrastEdit->text().toFloat(), 0.0f, 5.0f);
	float v = m_intensity;
	if(v > 1.0f)
		v = (v - 1.0f) / 4.0f + 1.0f;
	m_ui.contrastSlider->setValue(v * 100.0f);
	m_filter->setIntensity(m_intensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void ContrastAdjustWidget::apply()
{
	if(m_ui.contrastEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::contrast.cge.arg(m_intensity);
// 	content.android = FilterFormats::contrast.android.arg(m_intensity);
// 	content.flash = FilterFormats::contrast.flash.arg(m_intensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.contrastEdit->setText("");
}

void ContrastAdjustWidget::setContrast(int n)
{
	if(!checkFilter())
		return;

	m_intensity = n / 100.0f;
	if(m_intensity > 1.0f)
		m_intensity = 1.0f + (m_intensity - 1.0f) * 4.0f;
	m_ui.contrastEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void SaturationAdjustWidget::initSaturation()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.saturationEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.saturationEdit));
	m_ui.saturationSlider->setRange(0, 200);
	m_ui.saturationSlider->setValue(100);
	connect(m_ui.saturationSlider, SIGNAL(valueChanged(int)), SLOT(setSaturation(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool SaturationAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createSaturationFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void SaturationAdjustWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.saturationEdit->text().toFloat(), -1.0f, 5.0f);
	float v = m_intensity;
	if(v > 1.0f)
		v = (v - 1.0f) / 4.0f + 1.0f;
	m_ui.saturationSlider->setValue(v * 100.0f);
	m_filter->setIntensity(m_intensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void SaturationAdjustWidget::apply()
{
	if(m_ui.saturationEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::saturation.cge.arg(m_intensity);
// 	content.android = FilterFormats::saturation.android.arg(m_intensity);
// 	content.flash = FilterFormats::saturation.flash.arg(m_intensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.saturationEdit->setText("");
}

void SaturationAdjustWidget::setSaturation(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n / 100.0f;
	if(m_intensity > 1.0f)
		m_intensity = 1.0f + (m_intensity - 1.0f) * 4.0f;
	m_ui.saturationEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void ExposureAdjustWidget::initExposure()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.exposureEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.exposureEdit));
	m_ui.exposureSlider->setRange(-100, 100);
	m_ui.exposureSlider->setValue(0);
	connect(m_ui.exposureSlider, SIGNAL(valueChanged(int)), SLOT(setExposure(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ExposureAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createExposureFilter());
		m_mainWindow->appendFilter(m_filter);
	}

	return !!m_filter;
}

void ExposureAdjustWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.exposureEdit->text().toFloat(), -2.0f, 2.0f);	
	m_ui.exposureSlider->setValue(m_intensity * 50.0f);

	m_filter->setIntensity(m_intensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void ExposureAdjustWidget::apply()
{
	if(m_ui.exposureEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::exposure.cge.arg(m_intensity);
// 	content.android = FilterFormats::exposure.android.arg(m_intensity);
// 	content.flash = FilterFormats::exposure.flash.arg(m_intensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.exposureEdit->setText("");
}

void ExposureAdjustWidget::setExposure(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n / 50.0f;	
	m_ui.exposureEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void HueAdjustWidget::initHue()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.hueEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.hueEdit));
	m_ui.hueSlider->setRange(0, 360);
	m_ui.hueSlider->setValue(0);
	connect(m_ui.hueSlider, SIGNAL(valueChanged(int)), SLOT(setHue(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool HueAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createHueAdjustFilter());
		m_mainWindow->appendFilter(m_filter);
	}

	return !!m_filter;
}

void HueAdjustWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.hueEdit->text().toFloat(), 0.0f, (float)M_PI * 2.0f);
	m_ui.hueSlider->setValue(m_intensity * (180.0f / M_PI));

	m_filter->setHue(m_intensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void HueAdjustWidget::apply()
{
	if(m_ui.hueEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::hue.cge.arg(m_intensity);
// 	content.android = FilterFormats::hue.android.arg(m_intensity);
// 	content.flash = FilterFormats::hue.flash.arg(m_intensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.hueEdit->setText("");
}

void HueAdjustWidget::setHue(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n * (M_PI / 180.0f);
	m_ui.hueEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setHue(m_intensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void ShadowHighlightAdjustWidget::initShadowHighlight()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.shadowEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.shadowEdit));
	m_ui.highlightEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.highlightEdit));

	m_ui.shadowSlider->setRange(-200, 100);
	m_ui.highlightSlider->setRange(-100, 200);

	connect(m_ui.shadowSlider, SIGNAL(valueChanged(int)), SLOT(setShadow(int)));
	connect(m_ui.highlightSlider, SIGNAL(valueChanged(int)), SLOT(setHighlight(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ShadowHighlightAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createShadowHighlightFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void ShadowHighlightAdjustWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_shadowIntensity = CGE::CGE_MID(m_ui.shadowEdit->text().toFloat(), -200.0f, 100.0f);
	m_highlightIntensity = CGE::CGE_MID(m_ui.highlightEdit->text().toFloat(), -100.0f, 200.0f);	

	m_ui.shadowSlider->setValue(m_shadowIntensity);
	m_ui.highlightSlider->setValue(m_highlightIntensity);

	m_filter->setShadow(m_shadowIntensity);
	m_filter->setHighlight(m_highlightIntensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void ShadowHighlightAdjustWidget::apply()
{
	if(m_ui.shadowEdit->text().size() == 0 || m_ui.highlightEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::shadowHighlight.cge.arg(m_shadowIntensity).arg(m_highlightIntensity);
// 	content.android = FilterFormats::shadowHighlight.android.arg(m_shadowIntensity).arg(m_highlightIntensity);
// 	content.flash = FilterFormats::shadowHighlight.flash.arg(m_shadowIntensity).arg(m_highlightIntensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_shadowIntensity = 0.0f;
	m_highlightIntensity = 0.0f;
	m_ui.shadowEdit->setText("");
	m_ui.highlightEdit->setText("");
}

void ShadowHighlightAdjustWidget::setShadow(int n)
{
	if(!checkFilter())
		return;
	m_shadowIntensity = n;
	m_ui.shadowEdit->setText(QStringLiteral("%1").arg(m_shadowIntensity));
	m_filter->setShadow(m_shadowIntensity);
	m_mainWindow->runFilters();
}

void ShadowHighlightAdjustWidget::setHighlight(int n)
{
	if(!checkFilter())
		return;
	m_highlightIntensity = n;
	m_ui.highlightEdit->setText(QStringLiteral("%1").arg(m_highlightIntensity));
	m_filter->setHighlight(m_highlightIntensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void BlurSharpenAdjustWidget::initSharpenBlur()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.mixEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.mixEdit));
	m_ui.samplerEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.samplerEdit));

	m_ui.mixSlider->setRange(0, 200);
	m_ui.samplerSlider->setRange(10, 100);

	connect(m_ui.mixSlider, SIGNAL(valueChanged(int)), SLOT(setIntensity(int)));
	connect(m_ui.samplerSlider, SIGNAL(valueChanged(int)), SLOT(setSamplerScale(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool BlurSharpenAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createSharpenBlurSimpleBetterFilter());
		m_mainWindow->appendFilter(m_filter);
		m_filter->setIntensity(m_intensity);
		m_filter->setSamplerScale(m_samplerScale);
	}
	return !!m_filter;
}

void BlurSharpenAdjustWidget::preview()
{
	if(!checkFilter())
		return;

	m_mainWindow->disableRunFilters(true);

	m_intensity = CGE::CGE_MID(m_ui.mixEdit->text().toFloat(), 0.0f, 10.0f);
	m_samplerScale = CGE::CGE_MID(m_ui.samplerEdit->text().toFloat(), 1.0f, 10.0f);	

	float v = m_intensity * 100.0f;
	if(v > 100.0f)
	{
		v = (v - 100.0f) / 9.0f + 100.0f;
	}

	m_ui.mixSlider->setValue(v);
	m_ui.samplerSlider->setValue(m_samplerScale * 10.0f);

	m_filter->setIntensity(m_intensity);
	m_filter->setSamplerScale(m_samplerScale);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void BlurSharpenAdjustWidget::apply()
{
	if(m_ui.mixEdit->text().size() == 0 || m_ui.samplerEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	auto mode = m_intensity > 1.0 ? "sharpen" : "blur";

	FormatContent content;
	content.cge = FilterFormats::blurSharpen.cge.arg(mode).arg(m_intensity).arg(m_samplerScale);
// 	content.android = FilterFormats::blurSharpen.android.arg(mode).arg(m_intensity).arg(m_samplerScale);
// 	content.flash = FilterFormats::blurSharpen.flash.arg(mode).arg(m_intensity).arg(m_samplerScale);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 1.0f;
	m_samplerScale = 0.0f;
	m_ui.mixEdit->setText("");
	m_ui.samplerEdit->setText("");
}

void BlurSharpenAdjustWidget::setIntensity(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n / 100.0f;
	if(m_intensity > 1.0f)
		m_intensity = (m_intensity - 1.0f) * 9.0f + 1.0f;
	m_ui.mixEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

void BlurSharpenAdjustWidget::setSamplerScale(int n)
{
	if(!checkFilter())
		return;
	m_samplerScale = n / 10.0f;
	
	m_ui.samplerEdit->setText(QStringLiteral("%1").arg(m_samplerScale));
	m_filter->setSamplerScale(m_samplerScale);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void VignetteAdjustWidget::initVignette()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.vigStartEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigStartEdit));
	m_ui.vigRangeEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.vigRangeEdit));

	m_ui.vigStartSlider->setRange(0, 100);
	m_ui.vigRangeSlider->setRange(0, 100);
	m_ui.vigRangeSlider->setValue(100);

	connect(m_ui.vigStartSlider, SIGNAL(valueChanged(int)), SLOT(setStart(int)));
	connect(m_ui.vigRangeSlider, SIGNAL(valueChanged(int)), SLOT(setRange(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool VignetteAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createVignetteFilter());
		m_mainWindow->appendFilter(m_filter);
		m_filter->setVignette(m_vigStart, m_vigRange);
	}
	return !!m_filter;
}

void VignetteAdjustWidget::preview()
{
	if(!checkFilter())
		return;

	m_mainWindow->disableRunFilters(true);

	m_vigStart = CGE::CGE_MID(m_ui.vigStartEdit->text().toFloat(), 0.0f, 1.0f);
	m_vigRange = CGE::CGE_MID(m_ui.vigRangeEdit->text().toFloat(), 0.0f, 1.0f);	

	m_ui.vigStartSlider->setValue(m_vigStart * 100.0f);
	m_ui.vigRangeSlider->setValue(m_vigRange * 100.0f);

	m_filter->setVignette(m_vigStart, m_vigRange);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void VignetteAdjustWidget::apply()
{
	if(m_ui.vigStartEdit->text().size() == 0 || m_ui.vigRangeEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::vignette.cge.arg(m_vigStart).arg(m_vigRange);
// 	content.android = FilterFormats::vignette.android.arg(m_vigStart).arg(m_vigRange);
// 	content.flash = FilterFormats::vignette.flash.arg(m_vigStart).arg(m_vigRange);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_vigStart = 0.0f;
	m_vigRange = 1.0f;
	m_ui.vigStartEdit->setText("");
	m_ui.vigRangeEdit->setText("");
}

void VignetteAdjustWidget::setStart(int n)
{
	if(!checkFilter())
		return;
	m_vigStart = n / 100.0f;
	m_ui.vigStartEdit->setText(QStringLiteral("%1").arg(m_vigStart));
	m_filter->setVignette(m_vigStart, m_vigRange);
	m_mainWindow->runFilters();
}

void VignetteAdjustWidget::setRange(int n)
{
	if(!checkFilter())
		return;
	m_vigRange = n / 100.0f;
	m_ui.vigRangeEdit->setText(QStringLiteral("%1").arg(m_vigRange));
	m_filter->setVignette(m_vigStart, m_vigRange);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void HSLAdjustWidget::initHSL()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.hueEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.hueEdit));
	m_ui.saturationEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.saturationEdit));
	m_ui.luminanceEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.luminanceEdit));

	m_ui.hueSlider->setRange(-100, 100);
	m_ui.saturationSlider->setRange(-100, 100);
	m_ui.luminanceSlider->setRange(-100, 100);

	connect(m_ui.hueSlider, SIGNAL(valueChanged(int)), SLOT(setHue(int)));
	connect(m_ui.saturationSlider, SIGNAL(valueChanged(int)), SLOT(setSaturation(int)));
	connect(m_ui.luminanceSlider, SIGNAL(valueChanged(int)), SLOT(setLuminance(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool HSLAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createSaturationHSLFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void HSLAdjustWidget::preview()
{
	if(!checkFilter())
		return;

	m_mainWindow->disableRunFilters(true);

	m_hue = CGE::CGE_MID(m_ui.hueEdit->text().toFloat(), -1.0f, 1.0f);
	m_saturation = CGE::CGE_MID(m_ui.saturationEdit->text().toFloat(), -1.0f, 1.0f);	
	m_luminance = CGE::CGE_MID(m_ui.luminanceEdit->text().toFloat(), -1.0f, 1.0f);	

	m_ui.hueSlider->setValue(m_hue * 100.0f);
	m_ui.saturationSlider->setValue(m_saturation * 100.0f);
	m_ui.luminanceSlider->setValue(m_luminance * 100.0f);

	m_filter->setHue(m_hue);
	m_filter->setSaturation(m_saturation);
	m_filter->setLum(m_luminance);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void HSLAdjustWidget::apply()
{
	if(m_ui.hueEdit->text().size() == 0 || m_ui.saturationEdit->text().size() == 0 || m_ui.luminanceEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::hsl.cge.arg(m_hue).arg(m_saturation).arg(m_luminance);
// 	content.android = FilterFormats::hsl.android.arg(m_hue).arg(m_saturation).arg(m_luminance);
// 	content.flash = FilterFormats::hsl.flash.arg(m_hue).arg(m_saturation).arg(m_luminance);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_hue = 0.0f;
	m_saturation = 0.0f;
	m_luminance = 0.0f;
	m_ui.hueEdit->setText("");
	m_ui.saturationEdit->setText("");
	m_ui.luminanceEdit->setText("");
}

void HSLAdjustWidget::setHue(int n)
{
	if(!checkFilter())
		return;
	m_hue = n / 100.0f;
	m_ui.hueEdit->setText(QStringLiteral("%1").arg(m_hue));
	m_filter->setHue(m_hue);
	m_mainWindow->runFilters();
}

void HSLAdjustWidget::setSaturation(int n)
{
	if(!checkFilter())
		return;
	m_saturation = n / 100.0f;
	m_ui.saturationEdit->setText(QStringLiteral("%1").arg(m_saturation));
	m_filter->setSaturation(m_saturation);
	m_mainWindow->runFilters();
}

void HSLAdjustWidget::setLuminance(int n)
{
	if(!checkFilter())
		return;
	m_luminance = n / 100.0f;
	m_ui.luminanceEdit->setText(QStringLiteral("%1").arg(m_luminance));
	m_filter->setLum(m_luminance);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void HSVAdjustWidget::initHSV()
{
	QRegularExpression reg("[-0-9.]{1,20}");

	QLineEdit* edits[] = {
		m_ui.redEdit, 
		m_ui.greenEdit,
		m_ui.blueEdit,
		m_ui.yellowEdit,
		m_ui.magentaEdit,
		m_ui.cyanEdit
	};

	for(auto w : edits)
	{
		w->setValidator(new QRegularExpressionValidator(reg, w));
		connect(w, SIGNAL(returnPressed()), SLOT(activeSlider()));
	}
		
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

void HSVAdjustWidget::activeSlider()
{
	QWidget* focusW = focusWidget();
	QLineEdit* activeW = dynamic_cast<QLineEdit*>(focusW);

	if(activeW == nullptr)
		return;

	ToolSliderWidget* w = new ToolSliderWidget(this);
	w->setRange(-100, 100);
	w->setValue(activeW->text().toFloat() * 100);
	w->move(cursor().pos());
	w->setFunc([activeW, this](int value){
		activeW->setText(QStringLiteral("%1").arg(value / 100.0f));
		_preview(nullptr);
	});
	w->show();
	w->setFocus();
	w->update();
}

bool HSVAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = new CGE::CGESaturationHSVFilter;
		if(!m_filter->init())
		{
			CGE_LOG_ERROR("HSVAdjustWidget - CGESaturationHSVFilter init failed!\n");
			delete m_filter;
			m_filter = nullptr;
		}
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

bool HSVAdjustWidget::_preview(FormatContent* fmt)
{
	if(!checkFilter())
		return false;

	float red = CGE::CGE_MID(m_ui.redEdit->text().toFloat(), -1.0f, 1.0f);
	float green = CGE::CGE_MID(m_ui.greenEdit->text().toFloat(), -1.0f, 1.0f);
	float blue = CGE::CGE_MID(m_ui.blueEdit->text().toFloat(), -1.0f, 1.0f);
	float yellow = CGE::CGE_MID(m_ui.yellowEdit->text().toFloat(), -1.0f, 1.0f);
	float magenta = CGE::CGE_MID(m_ui.magentaEdit->text().toFloat(), -1.0f, 1.0f);
	float cyan = CGE::CGE_MID(m_ui.cyanEdit->text().toFloat(), -1.0f, 1.0f);

	if(fmt != nullptr && (red == 0.0f && green == 0.0f && blue == 0.0f && yellow == 0.0f && magenta == 0.0f && cyan == 0.0f))
	{
		QMessageBox::warning(this, "Invalid parameters!", "Please enter valid parameters!");
		return false;
	}

	if(!m_ui.redEdit->text().isEmpty())
		m_ui.redEdit->setText(QStringLiteral("%1").arg(red));
	if(!m_ui.greenEdit->text().isEmpty())
		m_ui.greenEdit->setText(QStringLiteral("%1").arg(green));
	if(!m_ui.blueEdit->text().isEmpty())
		m_ui.blueEdit->setText(QStringLiteral("%1").arg(blue));
	if(!m_ui.yellowEdit->text().isEmpty())
		m_ui.yellowEdit->setText(QStringLiteral("%1").arg(yellow));
	if(!m_ui.magentaEdit->text().isEmpty())
		m_ui.magentaEdit->setText(QStringLiteral("%1").arg(magenta));
	if(!m_ui.cyanEdit->text().isEmpty())
		m_ui.cyanEdit->setText(QStringLiteral("%1").arg(cyan));

	m_filter->setAdjustColors(red, green, blue, magenta, yellow, cyan);
	m_mainWindow->runFilters();

	if(fmt != nullptr)
	{
		fmt->cge = FilterFormats::hsv.cge.arg(red).arg(green).arg(blue).arg(magenta).arg(yellow).arg(cyan);
	}
	return true;
}

void HSVAdjustWidget::preview()
{
	_preview(nullptr);
}

void HSVAdjustWidget::apply()
{
	FormatContent content;
	if(!_preview(&content))
		return;

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_ui.redEdit->setText("");
	m_ui.greenEdit->setText("");
	m_ui.blueEdit->setText("");
	m_ui.magentaEdit->setText("");
	m_ui.yellowEdit->setText("");
	m_ui.cyanEdit->setText("");
}

//////////////////////////////////////////////////////////////////////////

void MonochromeAdjustWidget::initMonochrome()
{
	QRegularExpression reg("[-0-9.]{1,20}");

	QLineEdit* edits[] = {
		m_ui.redEdit, 
		m_ui.greenEdit,
		m_ui.blueEdit,
		m_ui.yellowEdit,
		m_ui.magentaEdit,
		m_ui.cyanEdit
	};

	for(auto w : edits)
	{
		w->setValidator(new QRegularExpressionValidator(reg, w));
		connect(w, SIGNAL(returnPressed()), SLOT(activeSlider()));
	}

	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

void MonochromeAdjustWidget::activeSlider()
{
	QWidget* focusW = focusWidget();
	QLineEdit* activeW = dynamic_cast<QLineEdit*>(focusW);

	if(activeW == nullptr)
		return;

	ToolSliderWidget* w = new ToolSliderWidget(this);
	w->setRange(-200, 300);
	w->setValue(activeW->text().toInt());
	w->move(cursor().pos());
	w->setFunc([activeW, this](int value){
		activeW->setText(QStringLiteral("%1").arg(value));
		_preview(nullptr);
	});
	w->show();
	w->setFocus();
	w->update();
}

bool MonochromeAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = new CGE::CGEMonochromeFilter;
		if(!m_filter->init())
		{
			CGE_LOG_ERROR("HSVAdjustWidget - CGESaturationHSVFilter init failed!\n");
			delete m_filter;
			m_filter = nullptr;
		}
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

bool MonochromeAdjustWidget::_preview(FormatContent* fmt)
{
	if(!checkFilter())
		return false;

	float red = CGE::CGE_MID(m_ui.redEdit->text().toFloat(), -200.0f, 300.0f);
	float green = CGE::CGE_MID(m_ui.greenEdit->text().toFloat(), -200.0f, 300.0f);
	float blue = CGE::CGE_MID(m_ui.blueEdit->text().toFloat(), -200.0f, 300.0f);
	float yellow = CGE::CGE_MID(m_ui.yellowEdit->text().toFloat(), -200.0f, 300.0f);
	float magenta = CGE::CGE_MID(m_ui.magentaEdit->text().toFloat(), -200.0f, 300.0f);
	float cyan = CGE::CGE_MID(m_ui.cyanEdit->text().toFloat(), -200.0f, 300.0f);

	if(!m_ui.redEdit->text().isEmpty())
		m_ui.redEdit->setText(QStringLiteral("%1").arg(red));
	if(!m_ui.greenEdit->text().isEmpty())
		m_ui.greenEdit->setText(QStringLiteral("%1").arg(green));
	if(!m_ui.blueEdit->text().isEmpty())
		m_ui.blueEdit->setText(QStringLiteral("%1").arg(blue));
	if(!m_ui.yellowEdit->text().isEmpty())
		m_ui.yellowEdit->setText(QStringLiteral("%1").arg(yellow));
	if(!m_ui.magentaEdit->text().isEmpty())
		m_ui.magentaEdit->setText(QStringLiteral("%1").arg(magenta));
	if(!m_ui.cyanEdit->text().isEmpty())
		m_ui.cyanEdit->setText(QStringLiteral("%1").arg(cyan));

	red /= 100.0f;
	green /= 100.0f;
	blue /= 100.0f;
	cyan /= 100.0f;
	magenta /= 100.0f;
	yellow /= 100.0f;

	m_filter->setRed(red);
	m_filter->setGreen(green);
	m_filter->setBlue(blue);
	m_filter->setCyan(cyan);
	m_filter->setMagenta(magenta);
	m_filter->setYellow(yellow);
	m_mainWindow->runFilters();

	if(fmt != nullptr)
	{
		fmt->cge = FilterFormats::monochrome.cge.arg(red).arg(green).arg(blue).arg(cyan).arg(magenta).arg(yellow);
	}
	return true;
}

void MonochromeAdjustWidget::preview()
{
	_preview(nullptr);
}

void MonochromeAdjustWidget::apply()
{
	FormatContent content;
	if(!_preview(&content))
		return;

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_ui.redEdit->setText("");
	m_ui.greenEdit->setText("");
	m_ui.blueEdit->setText("");
	m_ui.magentaEdit->setText("");
	m_ui.yellowEdit->setText("");
	m_ui.cyanEdit->setText("");
}

//////////////////////////////////////////////////////////////////////////

void WhiteBalanceAdjustWidget::initWhiteBalance()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.tempEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.tempEdit));
	m_ui.tintEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.tintEdit));

	m_ui.tempSlider->setRange(-100, 100);
	m_ui.tintSlider->setRange(0, 200);
	m_ui.tintSlider->setValue(100);

	connect(m_ui.tempSlider, SIGNAL(valueChanged(int)), SLOT(setTemperature(int)));
	connect(m_ui.tintSlider, SIGNAL(valueChanged(int)), SLOT(setTint(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool WhiteBalanceAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createWhiteBalanceFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void WhiteBalanceAdjustWidget::preview()
{
	if(!checkFilter())
		return;

	m_mainWindow->disableRunFilters(true);

	m_temp = CGE::CGE_MID(m_ui.tempEdit->text().toFloat(), -1.0f, 1.0f);
	m_tint = CGE::CGE_MID(m_ui.tintEdit->text().toFloat(), 0.0f, 5.0f);

	float t = m_tint;
	if(t > 1.0f)
		t = (t - 1.0f) / 4.0f + 1.0f;

	m_ui.tempSlider->setValue(m_temp * 100.0f);
	m_ui.tintSlider->setValue(t * 100.0f);

	m_filter->setTemperature(m_temp);
	m_filter->setTint(m_tint);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void WhiteBalanceAdjustWidget::apply()
{
	if(m_ui.tempEdit->text().size() == 0 || m_ui.tintEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::whiteBalance.cge.arg(m_temp).arg(m_tint);
// 	content.android = FilterFormats::whiteBalance.android.arg(m_temp).arg(m_tint);
// 	content.flash = FilterFormats::whiteBalance.flash.arg(m_temp).arg(m_tint);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_temp = 0.0f;
	m_tint = 1.0f;
	m_ui.tempEdit->setText("");
	m_ui.tintEdit->setText("");
}

void WhiteBalanceAdjustWidget::setTemperature(int n)
{
	if(!checkFilter())
		return;
	m_temp = n / 100.0f;
	m_ui.tempEdit->setText(QStringLiteral("%1").arg(m_temp));
	m_filter->setTemperature(m_temp);
	m_mainWindow->runFilters();
}

void WhiteBalanceAdjustWidget::setTint(int n)
{
	if(!checkFilter())
		return;
	m_tint = n / 100.0f;
	if(m_tint > 1.0f)
		m_tint = (m_tint - 1.0f) * 4.0f + 1.0f;
	m_ui.tintEdit->setText(QStringLiteral("%1").arg(m_tint));
	m_filter->setTint(m_tint);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////


void ColorBalanceAdjustWidget::initColorBalance()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.redShiftEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.redShiftEdit));
	m_ui.greenShiftEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.greenShiftEdit));
	m_ui.blueShiftEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.blueShiftEdit));

	connect(m_ui.redShiftSlider, SIGNAL(valueChanged(int)), SLOT(setRed(int)));
	connect(m_ui.greenShiftSlider, SIGNAL(valueChanged(int)), SLOT(setGreen(int)));
	connect(m_ui.blueShiftSlider, SIGNAL(valueChanged(int)), SLOT(setBlue(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ColorBalanceAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createColorBalanceFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void ColorBalanceAdjustWidget::preview()
{
	if(!checkFilter())
		return;

	m_mainWindow->disableRunFilters(true);

	m_red = CGE::CGE_MID(m_ui.redShiftEdit->text().toFloat(), -1.0f, 1.0f);
	m_green = CGE::CGE_MID(m_ui.greenShiftEdit->text().toFloat(), -1.0f, 1.0f);
	m_blue = CGE::CGE_MID(m_ui.blueShiftEdit->text().toFloat(), -1.0f, 1.0f);

	m_ui.redShiftSlider->setValue(m_red * 100.0f);
	m_ui.greenShiftSlider->setValue(m_green * 100.0f);
	m_ui.blueShiftSlider->setValue(m_blue * 100.0f);

	m_filter->setRedShift(m_red);
	m_filter->setGreenShift(m_green);
	m_filter->setBlueShift(m_blue);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void ColorBalanceAdjustWidget::apply()
{
	if(m_ui.redShiftEdit->text().size() == 0 && m_ui.greenShiftEdit->text().size() == 0 && m_ui.blueShiftEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::colorBalance.cge.arg(m_red).arg(m_green).arg(m_blue);
	// 	content.android = FilterFormats::whiteBalance.android.arg(m_temp).arg(m_tint);
	// 	content.flash = FilterFormats::whiteBalance.flash.arg(m_temp).arg(m_tint);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_red = 0.0f;
	m_green = 0.0f;
	m_blue  = 0.0f;
	m_ui.redShiftEdit->setText("");
	m_ui.greenShiftEdit->setText("");
	m_ui.blueShiftEdit->setText("");
}

void ColorBalanceAdjustWidget::setRed(int n)
{
	if(!checkFilter())
		return;
	m_red = n / 100.0f;
	m_ui.redShiftEdit->setText(QStringLiteral("%1").arg(m_red));
	m_filter->setRedShift(m_red);
	m_mainWindow->runFilters();
}

void ColorBalanceAdjustWidget::setGreen(int n)
{
	if(!checkFilter())
		return;
	m_green = n / 100.0f;
	m_ui.greenShiftEdit->setText(QStringLiteral("%1").arg(m_green));
	m_filter->setGreenShift(m_green);
	m_mainWindow->runFilters();
}

void ColorBalanceAdjustWidget::setBlue(int n)
{
	if(!checkFilter())
		return;
	m_blue = n / 100.0f;
	m_ui.blueShiftEdit->setText(QStringLiteral("%1").arg(m_blue));
	m_filter->setBlueShift(m_blue);
	m_mainWindow->runFilters();
}


//////////////////////////////////////////////////////////////////////////

void ColorLevelAdjustWidget::initColorLevel()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.darkEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.darkEdit));
	m_ui.lightEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.lightEdit));
	m_ui.gammaEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.gammaEdit));


	m_ui.lightSlider->setValue(100);
	m_ui.gammaSlider->setRange(0, 300);
	m_ui.gammaSlider->setValue(100);
	m_ui.lightEdit->setText("1");

	connect(m_ui.darkSlider, SIGNAL(valueChanged(int)), SLOT(setDark(int)));
	connect(m_ui.lightSlider, SIGNAL(valueChanged(int)), SLOT(setLight(int)));
	connect(m_ui.gammaSlider, SIGNAL(valueChanged(int)), SLOT(setGamma(int)));
	
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ColorLevelAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createColorLevelFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void ColorLevelAdjustWidget::preview()
{
	if(!checkFilter())
		return;

	m_mainWindow->disableRunFilters(true);

	m_dark = CGE::CGE_MID(m_ui.darkEdit->text().toFloat(), 0.0f, 1.0f);
	m_light = CGE::CGE_MID(m_ui.lightEdit->text().toFloat(), 0.0f, 1.0f);
	m_gamma = CGE::CGE_MID(m_ui.gammaEdit->text().toFloat(), 0.0f, 3.0f);

// 	if(m_light < m_dark)
// 		m_light = m_dark;

	m_ui.darkSlider->setValue(m_dark * 100.0f);
	m_ui.lightSlider->setValue(m_light * 100.0f);
	m_ui.gammaSlider->setValue(m_gamma * 100.0f);
	

	m_filter->setLevel(m_dark, m_light);
	m_filter->setGamma(m_gamma);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void ColorLevelAdjustWidget::apply()
{
	if(m_ui.darkEdit->text().size() == 0 || m_ui.lightEdit->text().size() == 0 || m_ui.gammaEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	if(m_dark >= m_light)
	{
		QMessageBox::information(m_mainWindow, QStringLiteral("Invalid Arguments"), "The 'dark' parameter of color level should not be greater than the 'light' parameter. Please correct it.");
		return;
	}

	FormatContent content;
	content.cge = FilterFormats::colorLevel.cge.arg(m_dark).arg(m_light).arg(m_gamma);
// 	content.android = FilterFormats::colorLevel.android.arg(m_dark).arg(m_light).arg(m_gamma);
// 	content.flash = FilterFormats::colorLevel.flash.arg(m_dark).arg(m_light).arg(m_gamma);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_dark = 0.0f;
	m_light = 1.0f;
	m_gamma = 1.0f;
	m_ui.darkEdit->setText("");
	m_ui.lightEdit->setText("");
	m_ui.gammaEdit->setText("");
}

void ColorLevelAdjustWidget::setDark(int n)
{
	if(!checkFilter())
		return;
	m_dark = n / 100.0f;
	m_ui.darkEdit->setText(QStringLiteral("%1").arg(m_dark));
	m_filter->setLevel(m_dark, m_light);
	m_mainWindow->runFilters();
}

void ColorLevelAdjustWidget::setLight(int n)
{
	if(!checkFilter())
		return;
	m_light = n / 100.0f;	
	m_ui.lightEdit->setText(QStringLiteral("%1").arg(m_light));
	m_filter->setLevel(m_dark, m_light);
	m_mainWindow->runFilters();
}

void ColorLevelAdjustWidget::setGamma(int n)
{
	if(!checkFilter())
		return;
	m_gamma = n / 100.0f;
	m_ui.gammaEdit->setText(QStringLiteral("%1").arg(m_gamma));
	m_filter->setGamma(m_gamma);
	m_mainWindow->runFilters();
}

void SelectiveColorAdjustWidget::initSelectiveColor()
{
	ColorArugments edits[9] = {
		{ m_ui.redCyanEdit, m_ui.redMagentaEdit, m_ui.redYellowEdit, m_ui.redBlackEdit }, //0
		{ m_ui.greenCyanEdit, m_ui.greenMagentaEdit, m_ui.greenYellowEdit, m_ui.greenBlackEdit }, //1
		{ m_ui.blueCyanEdit, m_ui.blueMagentaEdit, m_ui.blueYellowEdit, m_ui.blueBlackEdit }, //2
		{ m_ui.cyanCyanEdit, m_ui.cyanMagentaEdit, m_ui.cyanYellowEdit, m_ui.cyanBlackEdit }, //3
		{ m_ui.magentaCyanEdit, m_ui.magentaMagentaEdit, m_ui.magentaYellowEdit, m_ui.magentaBlackEdit }, //4
		{ m_ui.yellowCyanEdit, m_ui.yellowMagentaEdit, m_ui.yellowYellowEdit, m_ui.yellowBlackEdit }, //5
		{ m_ui.whiteCyanEdit, m_ui.whiteMagentaEdit, m_ui.whiteYellowEdit, m_ui.whiteBlackEdit }, //6
		{ m_ui.grayCyanEdit, m_ui.grayMagentaEdit, m_ui.grayYellowEdit, m_ui.grayBlackEdit }, //7
		{ m_ui.blackCyanEdit, m_ui.blackMagentaEdit, m_ui.blackYellowEdit, m_ui.blackBlackEdit } //8
	};

	QString argNames[9] = {
		"red", //0
		"green", //1
		"blue", //2
		"cyan", //3
		"magenta", //4
		"yellow", //5
		"white", //6
		"gray", //7
		"black", //8
	};

	std::copy(edits, edits + 9, m_colorArgs);
	std::copy(argNames, argNames + 9, m_argNames);

	QRegularExpression reg("[-0-9.]{1,20}");

	for(auto w : edits)
	{
		w.cyanEdit->setValidator(new QRegularExpressionValidator(reg, w.cyanEdit));
		connect(w.cyanEdit, SIGNAL(returnPressed()), SLOT(activeSlider()));

		w.magentaEdit->setValidator(new QRegularExpressionValidator(reg, w.magentaEdit));
		connect(w.magentaEdit, SIGNAL(returnPressed()), SLOT(activeSlider()));

		w.yellowEdit->setValidator(new QRegularExpressionValidator(reg, w.yellowEdit));
		connect(w.yellowEdit, SIGNAL(returnPressed()), SLOT(activeSlider()));

		w.blackEdit->setValidator(new QRegularExpressionValidator(reg, w.blackEdit));
		connect(w.blackEdit, SIGNAL(returnPressed()), SLOT(activeSlider()));
	}

	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

void SelectiveColorAdjustWidget::activeSlider()
{
	QWidget* focusW = focusWidget();
	QLineEdit* activeW = dynamic_cast<QLineEdit*>(focusW);

	if(activeW == nullptr)
		return;

	ToolSliderWidget* w = new ToolSliderWidget(this);
	w->setRange(-100, 100);
	w->setValue(activeW->text().toInt());
	w->move(cursor().pos());
	w->setFunc([activeW, this](int value){
		activeW->setText(QStringLiteral("%1").arg(value));
		_preview(nullptr);
		m_mainWindow->runFilters();
	});
	w->show();
	w->setFocus();
	w->update();
}

bool SelectiveColorAdjustWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createSelectiveColorFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void SelectiveColorAdjustWidget::setArg(int index, float cyan, float magenta, float yellow, float black)
{
	switch (index)
	{
	case 0:
		m_filter->setRed(cyan, magenta, yellow, black);
		break;
	case 1:
		m_filter->setGreen(cyan, magenta, yellow, black);
		break;
	case 2:
		m_filter->setBlue(cyan, magenta, yellow, black);
		break;
	case 3:
		m_filter->setCyan(cyan, magenta, yellow, black);
		break;
	case 4:
		m_filter->setMagenta(cyan, magenta, yellow, black);
		break;
	case 5:
		m_filter->setYellow(cyan, magenta, yellow, black);
		break;
	case 6:
		m_filter->setWhite(cyan, magenta, yellow, black);
		break;
	case 7:
		m_filter->setGray(cyan, magenta, yellow, black);
		break;
	case 8:
		m_filter->setBlack(cyan, magenta, yellow, black);
		break;
	default:
		CGE_LOG_ERROR("Invalid Arg Enum -- SelectiveColorAdjustWidget::setArg()!\n");
		break;
	}
}

bool SelectiveColorAdjustWidget::_preview(FormatContent* pContent)
{
	if(!checkFilter())
		return false;

	bool hasValidArgument = false;

	QString textFmt("%1");

	for(int i = 0; i != 9; ++i)
	{
		auto arg = m_colorArgs[i];
		float cyan = CGE::CGE_MID(arg.cyanEdit->text().toFloat() / 100.0f, -1.0f, 1.0f);
		float magenta = CGE::CGE_MID(arg.magentaEdit->text().toFloat() / 100.0f, -1.0f, 1.0f);
		float yellow = CGE::CGE_MID(arg.yellowEdit->text().toFloat() / 100.0f, -1.0f, 1.0f);
		float black = CGE::CGE_MID(arg.blackEdit->text().toFloat() / 100.0f, -1.0f, 1.0f);

		if(cyan != 0.0f || magenta != 0.0f || yellow != 0.0f || black != 0.0f)
		{
			setArg(i, cyan, magenta, yellow, black);
			if(pContent != nullptr)
			{
				pContent->cge += QStringLiteral(" %1(%2, %3, %4, %5)").arg(m_argNames[i]).arg(int(cyan * 100.0f)).arg(int(magenta * 100.0f)).arg(int(yellow * 100.0f)).arg(int(black * 100.0f));

				// TODO: handle pContent->android and similar platform-specific content here
			}
			hasValidArgument = true;
		}
	}

	return hasValidArgument;
}

void SelectiveColorAdjustWidget::preview()
{
	if(!_preview())
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	m_mainWindow->runFilters();
}

void SelectiveColorAdjustWidget::apply()
{
	FormatContent content;
	content.cge = "selcolor";

	if(!_preview(&content))
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	
	for(int i = 0; i != 9; ++i)
	{
		m_colorArgs[i].cyanEdit->setText("");
		m_colorArgs[i].magentaEdit->setText("");
		m_colorArgs[i].yellowEdit->setText("");
		m_colorArgs[i].blackEdit->setText("");
	}

	m_mainWindow->runFilters();
}

void ColorLookupAdjustWidget::initColorLookup()
{
	m_ui.workDirEdit->setText(QDir::currentPath());

	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
	connect(m_ui.workDirBtn, SIGNAL(clicked()), SLOT(setFolder()));
}

bool ColorLookupAdjustWidget::checkFilter()
{
	if (m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createLookupFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void ColorLookupAdjustWidget::setFolder()
{
	QString&& folderPath = QFileDialog::getExistingDirectory(this, QStringLiteral("Choose A Folder!"), QDir::currentPath());
	if (!folderPath.isEmpty())
	{
		QDir::setCurrent(folderPath);
		m_ui.workDirEdit->setText(folderPath);
		CGE_LOG_INFO("Working Dir Set To: %s\n", (const char*)folderPath.toLocal8Bit());
	}
	else
	{
		CGE_LOG_INFO("Cancel 'setFolder'");
	}
}

void ColorLookupAdjustWidget::preview()
{
	if (!checkFilter())
		return;

	QString filename = m_ui.filenameEdit->text();
	
	if (filename.isEmpty() || (m_filter->lookupTexture() != 0 && QString::compare(m_filename, filename) == 0))
	{
		return;
	}

	if (m_filter->lookupTexture() != 0)
	{
		glDeleteTextures(1, &m_filter->lookupTexture());
		m_filter->setLookupTexture(0);
	}

	m_filename = filename;

	QImage&& img = QImage(filename).convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly);

	if (img.isNull())
	{
		FILTER_COMMON_ERROR_MSG;
		return;
	}	

	GLuint texID = cgeGenTextureWithBuffer(img.bits(), img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE);

	if (texID == 0)
	{
		CGE_LOG_ERROR("Gen texture failed!\n");
		return;
	}

	m_filter->setLookupTexture(texID);
	m_mainWindow->runFilters();
}

void ColorLookupAdjustWidget::apply()
{
	preview();
	if (m_filter == nullptr || m_filter->lookupTexture() == 0)
		return;

	if (!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::lookup.cge.arg(m_filename);

	m_mainWindow->appendStep(m_filter, content);
	m_ui.filenameEdit->clear();
	m_filter = nullptr;
}

//////////////////////////////////////////////////////////////////////////

BasicAdjustWindow::BasicAdjustWindow(MainWindow* mainWindow, QWidget* parent) : MenuDialogCommon(parent), m_mainWindow(mainWindow), m_activeAdjustWidget(nullptr)
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::Tool);
	m_mainWindow->enableMenu(false);
	setFixedSize(size());
	connect(m_ui.listWidget, SIGNAL(currentRowChanged(int)), SLOT(switchItem(int)));
}

void BasicAdjustWindow::switchItem(int index)
{
	if(m_activeAdjustWidget)
	{
		delete m_activeAdjustWidget;
		m_activeAdjustWidget = nullptr;
	}

	switch (index)
	{
	case Brightness_Adjust:
		m_activeAdjustWidget = new BrightnessAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Contrast_Adjust:
		m_activeAdjustWidget = new ContrastAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Saturation_Adjust:
		m_activeAdjustWidget = new SaturationAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Monochrome_Adjust:
		m_activeAdjustWidget = new MonochromeAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Exposure_Adjust:
		m_activeAdjustWidget = new ExposureAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Hue_Adjust:
		m_activeAdjustWidget = new HueAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case ShadowHighlight_Adjust:
		m_activeAdjustWidget = new ShadowHighlightAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case BlurSharpen_Adjust:
		m_activeAdjustWidget = new BlurSharpenAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Vignette_Adjust:
		m_activeAdjustWidget = new VignetteAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case HSL_Adjust:
		m_activeAdjustWidget = new HSLAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case HSV_Adjust:
		m_activeAdjustWidget = new HSVAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case WhiteBalance_Adjust:
		m_activeAdjustWidget = new WhiteBalanceAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case ColorBalance_Adjust:
		m_activeAdjustWidget = new ColorBalanceAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case ColorLevel_Adjust:
		m_activeAdjustWidget = new ColorLevelAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case SelectiveColor_Adjust:
		m_activeAdjustWidget = new SelectiveColorAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case ColorLookup_Adjust:
		m_activeAdjustWidget = new ColorLookupAdjustWidget(m_mainWindow, m_ui.contentWidget);
		break;
	default:
		CGE_LOG_ERROR("Unexpected Adjust Item!\n");
		return ;
	}

	if(m_activeAdjustWidget)
	{
		m_activeAdjustWidget->setGeometry(0, 0, m_ui.contentWidget->width(), m_ui.contentWidget->height());
		m_activeAdjustWidget->show();
	}
}