/*
 * basicAdjustWidget.h
 *
 *  Created on: 2014-11-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#pragma once

#include "filterAdjustWidgetBase.h"

// CGE filter headers
#include "cgeFilterBasic.h"

// UI headers
#include "ui_basicAdjustWidget.h"
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

class BrightnessAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEBrightnessFilter, Ui::BrightnessAdjustWidget>
{
    Q_OBJECT
public:
    BrightnessAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setBrightness(int n);

private:
    void initBrightness();
    float m_intensity;
};

//////////////////////////////////////////////////////////////////////////

class ContrastAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEContrastFilter, Ui::ContrastAdjustWidget>
{
    Q_OBJECT
public:
    ContrastAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setContrast(int n);

private:
    void initContrast();
    float m_intensity;
};

//////////////////////////////////////////////////////////////////////////

class SaturationAdjustWidget : public FilterAdjustWidgetBase<CGE::CGESaturationFilter, Ui::SaturationAdjustWidget>
{
    Q_OBJECT
public:
    SaturationAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setSaturation(int n);

private:
    void initSaturation();
    float m_intensity;
};

//////////////////////////////////////////////////////////////////////////

class ExposureAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEExposureFilter, Ui::ExposureAdjustWidget>
{
    Q_OBJECT
public:
    ExposureAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setExposure(int n);

private:
    void initExposure();
    float m_intensity;
};

//////////////////////////////////////////////////////////////////////////

class HueAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEHueAdjustFilter, Ui::HueAdjsutWidget>
{
    Q_OBJECT
public:
    HueAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setHue(int n);

private:
    void initHue();
    float m_intensity;
};

//////////////////////////////////////////////////////////////////////////

class ShadowHighlightAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEShadowHighlightFilter, Ui::ShadowHighlightAdjustWidget>
{
    Q_OBJECT
public:
    ShadowHighlightAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setShadow(int n);
    void setHighlight(int n);

private:
    void initShadowHighlight();
    float m_shadowIntensity, m_highlightIntensity;
};

//////////////////////////////////////////////////////////////////////////

class BlurSharpenAdjustWidget : public FilterAdjustWidgetBase<CGE::CGESharpenBlurSimpleBetterFilter, Ui::blurSharpenAdjustWidget>
{
    Q_OBJECT
public:
    BlurSharpenAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;
    // Custom checkFilter: sets initial intensity/sampler on creation
    bool checkFilter();

protected slots:
    void preview();
    void apply();
    void setIntensity(int n);
    void setSamplerScale(int n);

private:
    void initSharpenBlur();
    float m_intensity, m_samplerScale;
};

//////////////////////////////////////////////////////////////////////////

class VignetteAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEVignetteFilter, Ui::VignetteAdjustWidget>
{
    Q_OBJECT
public:
    VignetteAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;
    // Custom checkFilter: sets initial vignette params on creation
    bool checkFilter();

protected slots:
    void preview();
    void apply();
    void setStart(int n);
    void setRange(int n);

private:
    void initVignette();
    float m_vigStart, m_vigRange;
};

//////////////////////////////////////////////////////////////////////////

class HSLAdjustWidget : public FilterAdjustWidgetBase<CGE::CGESaturationHSLFilter, Ui::HSLAdjustWidget>
{
    Q_OBJECT
public:
    HSLAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setHue(int n);
    void setSaturation(int n);
    void setLuminance(int n);

private:
    void initHSL();
    float m_hue, m_saturation, m_luminance;
};

//////////////////////////////////////////////////////////////////////////

class HSVAdjustWidget : public FilterAdjustWidgetBase<CGE::CGESaturationHSVFilter, Ui::HSVAdjustWidget>
{
    Q_OBJECT
public:
    HSVAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;
    bool _preview(FormatContent* fmt);

protected slots:
    void preview();
    void apply();
    void activeSlider();

private:
    void initHSV();
};

//////////////////////////////////////////////////////////////////////////

class MonochromeAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEMonochromeFilter, Ui::MonochromeAdjustWidget>
{
    Q_OBJECT
public:
    MonochromeAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;
    bool _preview(FormatContent* fmt);

protected slots:
    void preview();
    void apply();
    void activeSlider();

private:
    void initMonochrome();
};

//////////////////////////////////////////////////////////////////////////

class WhiteBalanceAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEWhiteBalanceFilter, Ui::WhitebalanceAdjustWidget>
{
    Q_OBJECT
public:
    WhiteBalanceAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setTemperature(int n);
    void setTint(int n);

private:
    void initWhiteBalance();
    float m_temp, m_tint;
};

//////////////////////////////////////////////////////////////////////////

class ColorBalanceAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEColorBalanceFilter, Ui::ColorBalanceAdjustWidget>
{
    Q_OBJECT
public:
    ColorBalanceAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setRed(int n);
    void setGreen(int n);
    void setBlue(int n);

private:
    void initColorBalance();
    float m_red, m_green, m_blue;
};

//////////////////////////////////////////////////////////////////////////

class ColorLevelAdjustWidget : public FilterAdjustWidgetBase<CGE::CGEColorLevelFilter, Ui::ColorLevelAdjustWidget>
{
    Q_OBJECT
public:
    ColorLevelAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void preview();
    void apply();
    void setDark(int n);
    void setLight(int n);
    void setGamma(int n);

private:
    void initColorLevel();
    float m_dark, m_light, m_gamma;
};

//////////////////////////////////////////////////////////////////////////

class SelectiveColorAdjustWidget : public FilterAdjustWidgetBase<CGE::CGESelectiveColorFilter, Ui::SelectiveColorWidget>
{
    Q_OBJECT
public:
    SelectiveColorAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

    struct ColorArguments
    {
        QLineEdit* cyanEdit;
        QLineEdit* magentaEdit;
        QLineEdit* yellowEdit;
        QLineEdit* blackEdit;
    };

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;
    void setArg(int index, float cyan, float magenta, float yellow, float black);
    bool _preview(FormatContent* s = nullptr);

protected slots:
    void preview();
    void apply();
    void activeSlider();

private:
    void initSelectiveColor();
    ColorArguments m_colorArgs[9];
    QString m_argNames[9];
};

//////////////////////////////////////////////////////////////////////////

class ColorLookupAdjustWidget : public FilterAdjustWidgetBase<CGE::CGELookupFilter, Ui::ColorLookupWidget>
{
    Q_OBJECT
public:
    ColorLookupAdjustWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

protected:
    CGE::CGEImageFilterInterfaceAbstract* createFilter() override;

protected slots:
    void setFolder();
    void preview();
    void apply();

private:
    void initColorLookup();
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
