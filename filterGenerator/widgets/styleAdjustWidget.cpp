/*
* styleAdjustWidget.cpp
*
*  Created on: 2015-2-6
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "styleAdjustWidget.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>

enum StyleAdjustEnum
{
	BilateralBlur_Adjust,
	Crosshatch_Adjust,
	Edge_Adjust,
	Emboss_Adjust,
	Halftone_Adjust,
	Haze_Adjust,
	Polkadot_Adjust,
	Lerpblur_Adjust,
	Sketch_Adjust,
	MaxValue_Adjust,
	MinValue_Adjust,
    MidValue_Adjust,
    Wave_Adjust,
	ColorMapping_Adjust,
};

void CrosshatchWidget::initCrosshatch()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.spacingEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.spacingEdit));
	m_ui.lineWidthEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.lineWidthEdit));

	connect(m_ui.spacingSlider, SIGNAL(valueChanged(int)), SLOT(setSpacing(int)));
	connect(m_ui.lineWidthSlider, SIGNAL(valueChanged(int)), SLOT(setLineWidth(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool CrosshatchWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createCrosshatchFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void CrosshatchWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_spacing = CGE::CGE_MID(m_ui.spacingEdit->text().toFloat(), 0.0f, 0.1f);
	m_ui.spacingSlider->setValue(m_spacing * 1000.0f);
	m_filter->setCrosshatchSpacing(m_spacing);

	m_lineWidth = CGE::CGE_MID(m_ui.lineWidthEdit->text().toFloat(), 0.0f, 0.01f);
	m_ui.lineWidthSlider->setValue(m_lineWidth * 10000.0f);
	m_filter->setLineWidth(m_lineWidth);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void CrosshatchWidget::apply()
{
	if(m_ui.spacingEdit->text().size() == 0 || m_ui.lineWidthEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::crossHatch.cge.arg(m_spacing).arg(m_lineWidth);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_spacing = 0.0f;
	m_lineWidth = 0.0f;
	m_ui.spacingEdit->setText("");
	m_ui.lineWidthEdit->setText("");
}

void CrosshatchWidget::setSpacing(int n)
{
	if(!checkFilter())
		return;
	m_spacing = n / 1000.0f;
	m_ui.spacingEdit->setText(QStringLiteral("%1").arg(m_spacing));
	m_filter->setCrosshatchSpacing(m_spacing);
	m_mainWindow->runFilters();
}

void CrosshatchWidget::setLineWidth(int n)
{
	if(!checkFilter())
		return;
	m_lineWidth = n / 10000.0f;
	m_ui.lineWidthEdit->setText(QStringLiteral("%1").arg(m_lineWidth));
	m_filter->setLineWidth(m_lineWidth);
	m_mainWindow->runFilters();
}

void EdgeWidget::initEdge()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.mixEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.mixEdit));
	m_ui.strideEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.strideEdit));

	connect(m_ui.mixSlider, SIGNAL(valueChanged(int)), SLOT(setMix(int)));
	connect(m_ui.strideSlider, SIGNAL(valueChanged(int)), SLOT(setStride(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool EdgeWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createEdgeSobelFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void EdgeWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_mix = CGE::CGE_MID(m_ui.mixEdit->text().toFloat(), 0.0f, 1.0f);
	m_ui.mixSlider->setValue(m_mix * 100.0f);
	m_filter->setIntensity(m_mix);

	m_stride = CGE::CGE_MID(m_ui.strideEdit->text().toFloat(), 0.0f, 5.0f);
	m_ui.strideSlider->setValue(m_stride * 20.0f);
	m_filter->setStride(m_stride);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void EdgeWidget::apply()
{
	if(m_ui.mixEdit->text().size() == 0 || m_ui.strideEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::edge.cge.arg(m_mix).arg(m_stride);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_mix = 0.0f;
	m_stride = 0.0f;
	m_ui.mixEdit->setText("");
	m_ui.strideEdit->setText("");
}

void EdgeWidget::setMix(int n)
{
	if(!checkFilter())
		return;
	m_mix = n / 100.0f;
	m_ui.mixEdit->setText(QStringLiteral("%1").arg(m_mix));
	m_filter->setIntensity(m_mix);
	m_mainWindow->runFilters();
}

void EdgeWidget::setStride(int n)
{
	if(!checkFilter())
		return;
	m_stride = n / 20.0f;
	m_ui.strideEdit->setText(QStringLiteral("%1").arg(m_stride));
	m_filter->setStride(m_stride);
	m_mainWindow->runFilters();
}

void EmbossWidget::initEmboss()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.mixEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.mixEdit));
	m_ui.strideEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.strideEdit));
	m_ui.angleEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.angleEdit));

	connect(m_ui.mixSlider, SIGNAL(valueChanged(int)), SLOT(setMix(int)));
	connect(m_ui.strideSlider, SIGNAL(valueChanged(int)), SLOT(setStride(int)));
	connect(m_ui.angleSlider, SIGNAL(valueChanged(int)), SLOT(setAngle(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool EmbossWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createEmbossFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void EmbossWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_mix = CGE::CGE_MID(m_ui.mixEdit->text().toFloat(), 0.0f, 1.0f);
	m_ui.mixSlider->setValue(m_mix * 100.0f);
	m_filter->setIntensity(m_mix);

	m_stride = CGE::CGE_MID(m_ui.strideEdit->text().toFloat(), 0.0f, 5.0f);
	m_ui.strideSlider->setValue(m_stride * 20.0f);
	m_filter->setStride(m_stride);

	m_angle = CGE::CGE_MID(m_ui.strideEdit->text().toFloat(), 0.0f, (float)M_PI * 2.0f);
	m_ui.strideSlider->setValue(m_angle * (100.0f / (M_PI * 2.0f)));
	m_filter->setStride(m_angle);

	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void EmbossWidget::apply()
{
	if(m_ui.mixEdit->text().size() == 0 || m_ui.strideEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::emboss.cge.arg(m_mix).arg(m_stride).arg(m_angle);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_mix = 0.0f;
	m_stride = 0.0f;
	m_angle = 0.0f;
	m_ui.mixEdit->setText("");
	m_ui.strideEdit->setText("");
	m_ui.angleEdit->setText("");
}

void EmbossWidget::setMix(int n)
{
	if(!checkFilter())
		return;
	m_mix = n / 100.0f;
	m_ui.mixEdit->setText(QStringLiteral("%1").arg(m_mix));
	m_filter->setIntensity(m_mix);
	m_mainWindow->runFilters();
}

void EmbossWidget::setStride(int n)
{
	if(!checkFilter())
		return;
	m_stride = n / 20.0f;
	m_ui.strideEdit->setText(QStringLiteral("%1").arg(m_stride));
	m_filter->setStride(m_stride);
	m_mainWindow->runFilters();
}

void EmbossWidget::setAngle(int n)
{
	if(!checkFilter())
		return;
	m_angle = n / 20.0f;
	m_ui.angleEdit->setText(QStringLiteral("%1").arg(m_angle));
	m_filter->setAngle(m_angle);
	m_mainWindow->runFilters();
}

void HalftoneWidget::initHalftone()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.halftoneEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.halftoneEdit));

	connect(m_ui.halftoneSlider, SIGNAL(valueChanged(int)), SLOT(setHalftone(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool HalftoneWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createHalftoneFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void HalftoneWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_dotSize = CGE::CGE_MID(m_ui.halftoneEdit->text().toFloat(), 0.0f, 20.0f);
	m_ui.halftoneSlider->setValue(m_dotSize * 5.0f);
	m_filter->setDotSize(m_dotSize);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void HalftoneWidget::apply()
{
	if(m_ui.halftoneEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::halftone.cge.arg(m_dotSize);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_dotSize = 0.0f;
	m_ui.halftoneEdit->setText("");
}

void HalftoneWidget::setHalftone(int n)
{
	if(!checkFilter())
		return;
	m_dotSize = n / 5.0f;
	m_ui.halftoneEdit->setText(QStringLiteral("%1").arg(m_dotSize));
	m_filter->setDotSize(m_dotSize);
	m_mainWindow->runFilters();
}

void HazeWidget::initHaze()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.distanceEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.distanceEdit));
	m_ui.slopeEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.slopeEdit));
	m_ui.redEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.redEdit));
	m_ui.greenEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.greenEdit));
	m_ui.blueEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.blueEdit));

	m_ui.distanceSlider->setRange(-100, 100);
	m_ui.slopeSlider->setRange(-100, 100);

	connect(m_ui.distanceSlider, SIGNAL(valueChanged(int)), SLOT(setDistance(int)));
	connect(m_ui.slopeSlider, SIGNAL(valueChanged(int)), SLOT(setSlope(int)));

	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool HazeWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createHazeFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void HazeWidget::preview()
{
	_useColor();

	m_mainWindow->disableRunFilters(true);

	m_distance = CGE::CGE_MID(m_ui.distanceEdit->text().toFloat(), -0.5f, 0.5f);
	m_ui.distanceSlider->setValue(m_distance * 200.0f);
	m_filter->setDistance(m_distance);

	m_slope = CGE::CGE_MID(m_ui.distanceEdit->text().toFloat(), -0.5f, 0.5f);
	m_ui.distanceSlider->setValue(m_slope * 200.0f);
	m_filter->setDistance(m_slope);

	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void HazeWidget::apply()
{
	if(m_ui.distanceEdit->text().size() == 0 || m_ui.slopeEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::haze.cge.arg(m_distance).arg(m_slope).arg(m_red).arg(m_green).arg(m_blue);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_distance = 0.0f;
	m_slope = 0.0f;	
	m_ui.distanceEdit->setText("");
	m_ui.slopeEdit->setText("");
}

void HazeWidget::_useColor()
{
	if(!checkFilter())
		return;
	m_red = CGE::CGE_MID(m_ui.redEdit->text().toFloat(), 0.0f, 1.0f);
	m_green = CGE::CGE_MID(m_ui.greenEdit->text().toFloat(), 0.0f, 1.0f);
	m_blue = CGE::CGE_MID(m_ui.blueEdit->text().toFloat(), 0.0f, 1.0f);
	m_filter->setHazeColor(m_red, m_green, m_blue);
}

void HazeWidget::setDistance(int n)
{
	_useColor();
	m_distance = n / 200.0f;
	m_ui.distanceEdit->setText(QStringLiteral("%1").arg(m_distance));
	m_filter->setDistance(m_distance);
	m_mainWindow->runFilters();
}

void HazeWidget::setSlope(int n)
{
	_useColor();
	m_slope = n / 200.0f;
	m_ui.slopeEdit->setText(QStringLiteral("%1").arg(m_slope));
	m_filter->setSlope(m_slope);
	m_mainWindow->runFilters();
}


void PolkadotWidget::initPolkadot()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.polkadotEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.polkadotEdit));

	connect(m_ui.polkadotSlider, SIGNAL(valueChanged(int)), SLOT(setDotScaling(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool PolkadotWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createPolkaDotFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void PolkadotWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_dotScaling = CGE::CGE_MID(m_ui.polkadotEdit->text().toFloat(), 0.0f, 1.0f);
	m_ui.polkadotSlider->setValue(m_dotScaling * 100.0f);
	m_filter->setDotSize(m_dotScaling);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void PolkadotWidget::apply()
{
	if(m_ui.polkadotEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::polkadot.cge.arg(m_dotScaling);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_dotScaling = 0.0f;
	m_ui.polkadotEdit->setText("");
}

void PolkadotWidget::setDotScaling(int n)
{
	if(!checkFilter())
		return;
	m_dotScaling = n / 100.0f;
	m_ui.polkadotEdit->setText(QStringLiteral("%1").arg(m_dotScaling));
	m_filter->setDotSize(m_dotScaling);
	m_mainWindow->runFilters();
}

void LerpblurWidget::initLerpblur()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.lerpBlurEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.lerpBlurEdit));

	connect(m_ui.lerpBlurSlider, SIGNAL(valueChanged(int)), SLOT(setBlurLevel(int)));
	connect(m_ui.blurBaseSlider, SIGNAL(valueChanged(int)), SLOT(setBlurBase(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool LerpblurWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createLerpblurFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void LerpblurWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.lerpBlurEdit->text().toFloat(), 0.0f, 1.0f);
	m_blurBase = CGE::CGE_MID(m_ui.blurBaseEdit->text().toFloat(), 0.6f, 2.0f);
	m_ui.lerpBlurSlider->setValue(m_intensity * 100.0f);
	m_ui.blurBaseSlider->setValue(m_blurBase * 100.0f);
	m_filter->setIntensity(m_intensity);
	m_filter->setMipmapBase(m_blurBase);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void LerpblurWidget::apply()
{
	if(m_ui.lerpBlurEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::lerpBlur.cge.arg(m_intensity).arg(m_blurBase);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.lerpBlurEdit->setText("");
}

void LerpblurWidget::setBlurLevel(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n / 100.0f;
	m_ui.lerpBlurEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

void LerpblurWidget::setBlurBase(int base)
{
	if(!checkFilter())
		return ;
	m_blurBase = base / 100.0f;
	m_filter->setMipmapBase(m_blurBase);
	m_ui.blurBaseEdit->setText(QStringLiteral("%1").arg(m_blurBase));
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void BilateralBlurWidget::initBlateralBlur()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.blurRadiusEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.blurRadiusEdit));
	m_ui.disFactorEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.disFactorEdit));

	connect(m_ui.blurRadiusSlider, SIGNAL(valueChanged(int)), SLOT(setBlurScale(int)));
	connect(m_ui.disFactorSlider, SIGNAL(valueChanged(int)), SLOT(setDistanceFactor(int)));
	connect(m_ui.repeatBox, SIGNAL(valueChanged(int)), SLOT(setRepeatTimes(int)));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
}

bool BilateralBlurWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = new CGE::CGEBilateralWrapperFilter();
		if(!m_filter->init())
		{
			delete m_filter;
			m_filter = nullptr;
		}
		m_filter->setBlurScale(m_blurScale);
		m_filter->setRepeatTimes(m_repeatTimes);
		m_filter->setDistanceNormalizationFactor(m_disFactor);
		m_mainWindow->appendFilter(m_filter);		
	}
	return !!m_filter;
}

void BilateralBlurWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	
	m_blurScale = CGE::CGE_MID(m_ui.blurRadiusEdit->text().toFloat(), -100.0f, 100.0f);
	m_ui.blurRadiusSlider->setValue(m_blurScale);

    m_disFactor = CGE::CGE_MID(m_ui.disFactorEdit->text().toFloat(), 0.0f, 20.0f);
	m_ui.disFactorSlider->setValue(m_disFactor * 10.0f);

	m_repeatTimes = m_ui.repeatBox->value();

	m_filter->setBlurScale(m_blurScale);
	m_filter->setDistanceNormalizationFactor(m_disFactor);
	m_filter->setRepeatTimes(m_repeatTimes);

	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void BilateralBlurWidget::apply()
{
	if(m_ui.blurRadiusEdit->text().size() == 0 || m_ui.disFactorEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::bilateralBlur.cge.arg(m_blurScale).arg(m_disFactor).arg(m_repeatTimes);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_blurScale = 0.0f;
	m_disFactor = 0.0f;
	m_ui.blurRadiusEdit->setText("");
	m_ui.disFactorEdit->setText("");
}

void BilateralBlurWidget::setBlurScale(int n)
{
	if(!checkFilter())
		return;
	m_blurScale = n;
	m_ui.blurRadiusEdit->setText(QStringLiteral("%1").arg(m_blurScale));
	m_filter->setBlurScale(m_blurScale);
	m_mainWindow->runFilters();
}

void BilateralBlurWidget::setDistanceFactor(int n)
{
	if(!checkFilter())
		return;
	m_disFactor = n / 10.0f;
	m_ui.disFactorEdit->setText(QStringLiteral("%1").arg(m_disFactor));
	m_filter->setDistanceNormalizationFactor(m_disFactor);
	m_mainWindow->runFilters();
}

void BilateralBlurWidget::setRepeatTimes(int n)
{
	if(!checkFilter())
		return;
	m_repeatTimes = n;
	m_filter->setRepeatTimes(m_repeatTimes);
	m_mainWindow->runFilters();
}

void SketchWidget::initSketch()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.sketchEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.sketchEdit));

	connect(m_ui.sketchSlider, SIGNAL(valueChanged(int)), SLOT(setIntensity(int)));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool SketchWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = static_cast<decltype(m_filter)>(CGE::createSketchFilter());
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void SketchWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);
	m_intensity = CGE::CGE_MID(m_ui.sketchEdit->text().toFloat(), -1.0f, 1.0f);
	m_ui.sketchSlider->setValue(m_intensity * 100.0f);
	m_filter->setIntensity(m_intensity);
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void SketchWidget::apply()
{
	if(m_ui.sketchEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return ;
	}

	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::sketch.cge.arg(m_intensity);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_intensity = 0.0f;
	m_ui.sketchEdit->setText("");
}

void SketchWidget::setIntensity(int n)
{
	if(!checkFilter())
		return;
	m_intensity = n / 100.0f;
	m_ui.sketchEdit->setText(QStringLiteral("%1").arg(m_intensity));
	m_filter->setIntensity(m_intensity);
	m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void MaxValueWidget::initMaxValue()
{
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
	m_ui.descriptionEdit->setPlainText("Max Value Filter");
}

bool MaxValueWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = new CGE::CGEMaxValueFilter3x3;;
		if(!m_filter->init())
		{
			delete m_filter;
			m_filter = nullptr;
		}
		else
		{
			m_mainWindow->appendFilter(m_filter);
		}		
	}
	return !!m_filter;
}

void MaxValueWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->runFilters();
}

void MaxValueWidget::apply()
{
	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::max.cge;

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
}

//////////////////////////////////////////////////////////////////////////

void MinValueWidget::initMinValue()
{
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
	m_ui.descriptionEdit->setPlainText("Min Value Filter");
}

bool MinValueWidget::checkFilter()
{
	if(m_filter == nullptr)
	{
		m_filter = new CGE::CGEMinValueFilter3x3;;
		if(!m_filter->init())
		{
			delete m_filter;
			m_filter = nullptr;
		}
		else
		{
			m_mainWindow->appendFilter(m_filter);
		}		
	}
	return !!m_filter;
}

void MinValueWidget::preview()
{
	if(!checkFilter())
		return;
	m_mainWindow->runFilters();
}

void MinValueWidget::apply()
{
	if(!m_filter)
	{
		preview();
	}

	FormatContent content;
	content.cge = FilterFormats::min.cge;

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
}

//////////////////////////////////////////////////////////////////////////

void WaveWidget::initWave()
{
    QRegularExpression reg("[-0-9.]{1,20}");
    m_ui.motionEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.motionEdit));
    m_ui.angleEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.angleEdit));
    m_ui.strengthEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.strengthEdit));

    m_ui.motionSlider->setRange(-100, 100);

    connect(m_ui.motionSlider, SIGNAL(valueChanged(int)), SLOT(setMotion(int)));
    connect(m_ui.angleSlider, SIGNAL(valueChanged(int)), SLOT(setAngle(int)));
    connect(m_ui.strengthSlider, SIGNAL(valueChanged(int)), SLOT(setStrength(int)));

    connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
    connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool WaveWidget::checkFilter()
{
    if(m_filter == nullptr)
    {
        m_filter = static_cast<decltype(m_filter)>(CGE::createDynamicWaveFilter());
        m_mainWindow->appendFilter(m_filter);
    }
    return !!m_filter;
}

void WaveWidget::preview()
{
    if(!checkFilter())
        return;
    m_mainWindow->disableRunFilters(true);

    m_motion = CGE::CGE_MID(m_ui.motionEdit->text().toFloat(), -25.0f, 25.0f);
    m_ui.motionSlider->setValue(m_motion * 4.0f);
    m_filter->setWaveMotion(m_motion);

    m_angle = CGE::CGE_MID(m_ui.angleEdit->text().toFloat(), 0.0f, 50.0f);
    m_ui.angleSlider->setValue(m_angle * 2.0f);
    m_filter->setWaveAngle(m_angle);

    m_strength = CGE::CGE_MID(m_ui.strengthEdit->text().toFloat(), 0.0f, 0.1f);
    m_ui.strengthSlider->setValue(m_strength * 1000.0f);
    m_filter->setStrength(m_strength);

    m_mainWindow->disableRunFilters(false);
    m_mainWindow->runFilters();
}

void WaveWidget::apply()
{
    if(m_ui.motionEdit->text().size() == 0 ||
        m_ui.angleEdit->text().size() == 0 ||
        m_ui.strengthEdit->text().size() == 0)
    {
        FILTER_COMMON_ERROR_MSG;
        return ;
    }

    if(!m_filter)
    {
        preview();
    }

    FormatContent content;
    content.cge = FilterFormats::wave.cge.arg(m_motion).arg(m_angle).arg(m_strength);

    m_mainWindow->appendStep(m_filter, content);
    m_filter = nullptr;
    m_motion = 0.0f;
    m_angle = 0.0f;
    m_strength = 0.0f;
    m_ui.motionEdit->setText("");
    m_ui.angleEdit->setText("");
    m_ui.strengthEdit->setText("");
}

void WaveWidget::setMotion(int n)
{
    if(!checkFilter())
        return;
    m_motion = n / 4.0f;
    m_ui.motionEdit->setText(QStringLiteral("%1").arg(m_motion));
    m_filter->setWaveMotion(m_motion);
    m_mainWindow->runFilters();
}

void WaveWidget::setAngle(int n)
{
    if(!checkFilter())
        return;
    m_angle = n / 2.0f;
    m_ui.angleEdit->setText(QStringLiteral("%1").arg(m_angle));
    m_filter->setWaveAngle(m_angle);
    m_mainWindow->runFilters();
}

void WaveWidget::setStrength(int n)
{
    if(!checkFilter())
        return;
    m_strength = n / 1000.0f;
    m_ui.strengthEdit->setText(QStringLiteral("%1").arg(m_strength));
    m_filter->setStrength(m_strength);
    m_mainWindow->runFilters();
}

//////////////////////////////////////////////////////////////////////////

void ColorMappingWidget::initColorMapping()
{
	QRegularExpression reg("[-0-9.]{1,20}");
	m_ui.widthEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.widthEdit));
	m_ui.heightEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.heightEdit));
	m_ui.unitWidthEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.unitWidthEdit));
	m_ui.unitHeightEdit->setValidator(new QRegularExpressionValidator(reg, m_ui.unitHeightEdit));

	m_ui.folderEdit->setText(QDir::currentPath());

	connect(m_ui.folderBtn, SIGNAL(clicked()), SLOT(setCurrentDir()));
	connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

bool ColorMappingWidget::checkFilter()
{
	if (m_filter == nullptr)
	{
		m_filter = CGE::CGEColorMappingFilter::createWithMode(CGE::CGEColorMappingFilter::MAPINGMODE_DEFAULT);
		m_mainWindow->appendFilter(m_filter);
	}
	return !!m_filter;
}

void ColorMappingWidget::setCurrentDir()
{
	QString&& filename = QFileDialog::getExistingDirectory(this, QStringLiteral("Choose A Folder!"), m_ui.folderEdit->text());
	if(!filename.isEmpty())
	{
		QDir::setCurrent(filename);
		m_ui.folderEdit->setText(filename);
		CGE_LOG_INFO("Working dir set to: %s\n", filename.toLocal8Bit().constData());
	}
	else
	{
		CGE_LOG_INFO("Dir setting canceled.\n");
	}
}

void ColorMappingWidget::preview()
{
	using namespace CGE;
	if (!checkFilter())
		return;
	m_mainWindow->disableRunFilters(true);

	m_width = m_ui.widthEdit->text().toInt();
	m_height = m_ui.heightEdit->text().toInt();
	m_unitWidth = m_ui.unitWidthEdit->text().toInt();
	m_unitHeight = m_ui.unitHeightEdit->text().toInt();
	QString&& str = m_ui.filenameEdit->text();

	GLuint texID = m_filter->getTexture();
	CGESizei sz = m_filter->getTexSize();

	if(str != m_filename || texID == 0)
	{
		QImage&& img = QImage(str).convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::ColorOnly);

		if(img.isNull())
		{
			QMessageBox::information(this, "Image Not Found", "Image does not exist! Please check whether the directory is set correctly!");
			return ;
		}

		m_filename = str;

		if(texID != 0)
			glDeleteTextures(1, &texID);

		texID = cgeGenTextureWithBuffer(img.bits(), img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE);
		sz.set(img.width(), img.height());
	}

	m_filter->clearPushing();

	float weight = 0.0f;

	for(int i = 0; i != m_unitHeight; ++i)
	{
		for(int j = 0; j != m_unitWidth; ++j)
		{
			CGEColorMappingFilter::MappingArea ma = {
				Vec4f(j / (float)m_unitWidth, i / (float)m_unitHeight, 1.0f / m_unitWidth, 1.0f / m_unitHeight),
				weight
			};

			weight += 1.0f / (m_unitWidth * m_unitHeight - 1);

			m_filter->pushMapingArea(ma);
		}
	}

	m_filter->endPushing();
	m_filter->setupMapping(texID, sz.width, sz.height, m_width, m_height);
	
	m_mainWindow->disableRunFilters(false);
	m_mainWindow->runFilters();
}

void ColorMappingWidget::apply()
{
	if (m_ui.filenameEdit->text().size() == 0 ||
		m_ui.widthEdit->text().size() == 0 ||
		m_ui.heightEdit->text().size() == 0 ||
		m_ui.unitWidthEdit->text().size() == 0 || 
		m_ui.unitHeightEdit->text().size() == 0)
	{
		FILTER_COMMON_ERROR_MSG;
		return;
	}

	preview();

	if(m_filename.isEmpty() || m_filter == nullptr || m_filter->getTexture() == 0)
		return ;

	FormatContent content;
	content.cge = FilterFormats::colorMapping.cge.arg(m_filename).arg(m_width).arg(m_height).arg(m_unitWidth).arg(m_unitHeight);

	m_mainWindow->appendStep(m_filter, content);
	m_filter = nullptr;
	m_ui.filenameEdit->setText("");
}

//////////////////////////////////////////////////////////////////////////

StyleAdjustWindow::StyleAdjustWindow(MainWindow* mainwindow, QWidget* parent) : MenuDialogCommon(parent), m_mainWindow(mainwindow), m_activeAdjustWidget(nullptr)
{
	m_ui.setupUi(this);
	setWindowFlags(Qt::Tool);
	m_mainWindow->enableMenu(false);
	setFixedSize(size());
	connect(m_ui.listWidget, SIGNAL(currentRowChanged(int)), SLOT(switchItem(int)));
}

void StyleAdjustWindow::switchItem(int index)
{
	if(m_activeAdjustWidget)
	{
		CGE_DELETE(m_activeAdjustWidget);
	}

	switch (StyleAdjustEnum(index))
	{
	case BilateralBlur_Adjust:
		m_activeAdjustWidget = new BilateralBlurWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Crosshatch_Adjust:
		m_activeAdjustWidget = new CrosshatchWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Edge_Adjust:
		m_activeAdjustWidget = new EdgeWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Emboss_Adjust:
		m_activeAdjustWidget = new EmbossWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Halftone_Adjust:
		m_activeAdjustWidget = new HalftoneWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Haze_Adjust:
		m_activeAdjustWidget = new HazeWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Polkadot_Adjust:
		m_activeAdjustWidget = new PolkadotWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Lerpblur_Adjust:
		m_activeAdjustWidget = new LerpblurWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case Sketch_Adjust:
		m_activeAdjustWidget = new SketchWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case MaxValue_Adjust:
		m_activeAdjustWidget = new MaxValueWidget(m_mainWindow, m_ui.contentWidget);
		break;
	case MinValue_Adjust:
		m_activeAdjustWidget = new MinValueWidget(m_mainWindow, m_ui.contentWidget);
		break;
    case Wave_Adjust:
        m_activeAdjustWidget = new WaveWidget(m_mainWindow, m_ui.contentWidget);
        break;
	case ColorMapping_Adjust:
		m_activeAdjustWidget = new ColorMappingWidget(m_mainWindow, m_ui.contentWidget);
		break;
	default:
		break;
	}

	if(m_activeAdjustWidget)
	{
		m_activeAdjustWidget->setGeometry(0, 0, m_ui.contentWidget->width(), m_ui.contentWidget->height());
		m_activeAdjustWidget->show();
	}
}
