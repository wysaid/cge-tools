/*
* basicAdjustWidget.h
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifndef _BASIC_ADJUST_WIDGET_
#define _BASIC_ADJUST_WIDGET_

#include "mainwindow.h"
#include "ui_basicAdjustWidget.h"

// CGE filter headers
#include "cgeFilterBasic.h"

#include "ui_brightnessAdjustWidget.h"
#include "ui_contrastAdjustWidget.h"
#include "ui_saturationAdjustWidget.h"
#include "ui_shadowHighlightWidget.h"
#include "ui_blurSharpenAdjustWidget.h"
#include "ui_vignetteAdjustWidget.h"
#include "ui_HSLAdjustWidget.h"
#include "ui_HSVAdjustWidget.h"
#include "ui_whiteBalanceAdjustWidget.h"
#include "ui_colorLevelAdjustWidget.h"
#include "ui_selectiveColorAdjustWidget.h"
#include "ui_exposureAdjustWidget.h"
#include "ui_hueAdjustWidget.h"
#include "ui_colorBalanceAdjustWidget.h"
#include "ui_monochromeAdjustWidget.h"
#include "ui_colorLookupWidget.h"

class MainWindow;
class MenuDialogCommon;

//////////////////////////////////////////////////////////////////////////

class BrightnessAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	BrightnessAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f) { m_ui.setupUi(this); initBrightness(); }
	~BrightnessAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initBrightness();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setBrightness(int n);

private:
	Ui::BrightnessAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEBrightnessFilter* m_filter;
	float m_intensity;
};

//////////////////////////////////////////////////////////////////////////

class ContrastAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	ContrastAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f)
	{
		m_ui.setupUi(this);
		initContrast();
	}

	~ContrastAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initContrast();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setContrast(int n);

private:
	Ui::ContrastAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEContrastFilter* m_filter;
	float m_intensity;
};

class SaturationAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	SaturationAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f) { m_ui.setupUi(this); initSaturation(); }
	~SaturationAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initSaturation();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setSaturation(int n);

private:
	Ui::SaturationAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGESaturationFilter* m_filter;
	float m_intensity;
};

class ExposureAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	ExposureAdjustWidget (MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f)
	{
		m_ui.setupUi(this);
		initExposure();
	}

	~ExposureAdjustWidget ()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initExposure();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setExposure(int n);

private:
	Ui::ExposureAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEExposureFilter* m_filter;
	float m_intensity;
};

class HueAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	HueAdjustWidget (MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f)
	{
		m_ui.setupUi(this);
		initHue();
	}

	~HueAdjustWidget ()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initHue();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setHue(int n);

private:
	Ui::HueAdjsutWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEHueAdjustFilter* m_filter;
	float m_intensity;
};

class ShadowHighlightAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	ShadowHighlightAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_shadowIntensity(0.0f), m_highlightIntensity(0.0f)
	{
		m_ui.setupUi(this);
		initShadowHighlight();
	}

	~ShadowHighlightAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initShadowHighlight();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setShadow(int n);
	void setHighlight(int n);

private:
	Ui::ShadowHighlightAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEShadowHighlightFilter * m_filter;
	float m_shadowIntensity, m_highlightIntensity;
};

class BlurSharpenAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	BlurSharpenAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(1.0f), m_samplerScale(0.0f) { m_ui.setupUi(this); initSharpenBlur(); }
	~BlurSharpenAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initSharpenBlur();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setIntensity(int n);
	void setSamplerScale(int n);

private:
	Ui::blurSharpenAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGESharpenBlurSimpleBetterFilter* m_filter;
	float m_intensity, m_samplerScale;
};

class VignetteAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	VignetteAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_vigStart(0.0f), m_vigRange(1.0f) { m_ui.setupUi(this); initVignette(); }
	~VignetteAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initVignette();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setStart(int n);
	void setRange(int n);

private:
	Ui::VignetteAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEVignetteFilter* m_filter;
	float m_vigStart, m_vigRange;
};

class HSLAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	HSLAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_hue(0.0f), m_saturation(0.0f), m_luminance(0.0f) { m_ui.setupUi(this); initHSL(); }
	~HSLAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initHSL();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setHue(int n);
	void setSaturation(int n);
	void setLuminance(int n);

private:
	Ui::HSLAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGESaturationHSLFilter* m_filter;
	float m_hue, m_saturation, m_luminance;
};

class HSVAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	HSVAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr) { m_ui.setupUi(this); initHSV(); }
	~HSVAdjustWidget()
	{
		if(m_filter)
			m_mainWindow->removeFilter(m_filter);
	}

protected:

	void initHSV();
	bool checkFilter();

	bool _preview(FormatContent* fmt);

protected slots:

	void preview();
	void apply();

	void activeSlider();

private:
	Ui::HSVAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGESaturationHSVFilter* m_filter;
};

class MonochromeAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	MonochromeAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr) { m_ui.setupUi(this); initMonochrome(); }
	~MonochromeAdjustWidget()
	{
		if(m_filter)
			m_mainWindow->removeFilter(m_filter);
	}

protected:

	void initMonochrome();
	bool checkFilter();

	bool _preview(FormatContent* fmt);

protected slots:

	void preview();
	void apply();

	void activeSlider();

private:
	Ui::MonochromeAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEMonochromeFilter* m_filter;
};

class WhiteBalanceAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	WhiteBalanceAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_temp(0.0f), m_tint(1.0f) { m_ui.setupUi(this); initWhiteBalance(); }
	~WhiteBalanceAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initWhiteBalance();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setTemperature(int n);
	void setTint(int n);

private:
	Ui::WhitebalanceAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEWhiteBalanceFilter* m_filter;
	float m_temp, m_tint;
};

class ColorBalanceAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	ColorBalanceAdjustWidget (MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_red(0.0f), m_green(0.0f), m_blue(0.0f) { m_ui.setupUi(this); initColorBalance(); }
	~ColorBalanceAdjustWidget ()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initColorBalance();
	bool checkFilter();

protected slots:

	void preview();
	void apply();

	void setRed(int n);
	void setGreen(int n);
	void setBlue(int n);

private:
	Ui::ColorBalanceAdjustWidget  m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEColorBalanceFilter* m_filter;
	float m_red, m_green, m_blue;
};

class ColorLevelAdjustWidget : public QWidget
{
	Q_OBJECT
public:
    ColorLevelAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_dark(0.0f), m_light(1.0f), m_gamma(1.0f) { m_ui.setupUi(this); initColorLevel(); }
	~ColorLevelAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initColorLevel();
	bool checkFilter();

protected slots:

	void preview();
	void apply();
	void setDark(int n);
	void setLight(int n);
	void setGamma(int n);

private:
	Ui::ColorLevelAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEColorLevelFilter* m_filter;
	float m_dark, m_light, m_gamma;
};

class SelectiveColorAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	SelectiveColorAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr) { m_ui.setupUi(this); initSelectiveColor(); }
	~SelectiveColorAdjustWidget()
	{
		if(m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

	typedef struct ColorArugments
	{
		QLineEdit* cyanEdit;
		QLineEdit* magentaEdit;
		QLineEdit* yellowEdit;
		QLineEdit* blackEdit;
	}ColorArugments;

protected:

	void initSelectiveColor();
	bool checkFilter();

	void setArg(int index, float cyan, float magenta, float yellow, float black);
	bool _preview(FormatContent* s = nullptr);


protected slots:

	void preview();
	void apply();

	void activeSlider();

private:
	Ui::SelectiveColorWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGESelectiveColorFilter* m_filter;
	ColorArugments m_colorArgs[9];
	QString m_argNames[9];
};


class ColorLookupAdjustWidget : public QWidget
{
	Q_OBJECT
public:
	ColorLookupAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr){ m_ui.setupUi(this); initColorLookup(); }
	~ColorLookupAdjustWidget()
	{
		if (m_filter)
		{
			m_mainWindow->removeFilter(m_filter);
		}
	}

protected:

	void initColorLookup();
	bool checkFilter();

protected slots:
	void setFolder();
	void preview();
	void apply();

private:
	Ui::ColorLookupWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGELookupFilter* m_filter;
	QString m_filename;
};

//////////////////////////////////////////////////////////////////////////

class BasicAdjustWindow : public MenuDialogCommon
{
	Q_OBJECT
public:
	BasicAdjustWindow(MainWindow* mainWindow, QWidget* parent = nullptr);
	~BasicAdjustWindow()
	{
		CGE_LOG_INFO("BasicAdjustWindow closed!\n");
		m_mainWindow->enableMenu(true);
	}

public slots:
	void switchItem(int index);

private:
	Ui::BasicAdjustWidget m_ui;
	MainWindow* m_mainWindow;
	QWidget* m_activeAdjustWidget;
};


#endif
