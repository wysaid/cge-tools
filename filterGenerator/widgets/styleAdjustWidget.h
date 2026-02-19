/*
 * styleAdjustWidget.h
 *
 *  Created on: 2015-2-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _STYLE_ADJUST_WIDGET_
#define _STYLE_ADJUST_WIDGET_

#include "mainwindow.h"

// CGE filter headers
#include "cgeFilterBasic.h"
#include "cgeAdvancedEffects.h"
#include "cgeBilateralBlurFilter.h"
#include "cgeColorMappingFilter.h"
#include "cgeCrosshatchFilter.h"
#include "cgeDynamicWaveFilter.h"
#include "cgeEdgeFilter.h"
#include "cgeEmbossFilter.h"
#include "cgeHalftoneFilter.h"
#include "cgeHazeFilter.h"
#include "cgeLerpblurFilter.h"
#include "cgeMaxValueFilter.h"
#include "cgeMinValueFilter.h"
#include "cgePolkaDotFilter.h"
#include "cgeSketchFilter.h"

#include "ui_styleAdjustWidget.h"
#include "ui_crosshatchWidget.h"
#include "ui_edgeWidget.h"
#include "ui_embossWidget.h"
#include "ui_halftoneWidget.h"
#include "ui_hazeWidget.h"
#include "ui_polkadotWidget.h"
#include "ui_bilateralBlurWidget.h"
#include "ui_LerpBlurWidget.h"
#include "ui_sketchWidget.h"
#include "ui_waveAdjustWidget.h"
#include "ui_previewApplyWidget.h"
#include "ui_colorMappingWidget.h"

class MainWindow;
class MenuDialogCommon;
class QWidget;

class CrosshatchWidget : public QWidget
{
    Q_OBJECT
public:
    CrosshatchWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_spacing(0.0f), m_lineWidth(0.0f)
    {
        m_ui.setupUi(this);
        initCrosshatch();
    }
    ~CrosshatchWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initCrosshatch();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setSpacing(int n);
    void setLineWidth(int n);

private:
    Ui::CrosshatchWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGECrosshatchFilter* m_filter;
    float m_spacing, m_lineWidth;
};

class EdgeWidget : public QWidget
{
    Q_OBJECT
public:
    EdgeWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_mix(0.0f), m_stride(0.0f)
    {
        m_ui.setupUi(this);
        initEdge();
    }
    ~EdgeWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initEdge();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setMix(int n);
    void setStride(int n);

private:
    Ui::EdgeWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEEdgeSobelFilter* m_filter;
    float m_mix, m_stride;
};

class EmbossWidget : public QWidget
{
    Q_OBJECT
public:
    EmbossWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_mix(0.0f), m_stride(0.0f), m_angle(0.0f)
    {
        m_ui.setupUi(this);
        initEmboss();
    }
    ~EmbossWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initEmboss();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setMix(int n);
    void setStride(int n);
    void setAngle(int n);

private:
    Ui::EmbossWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEEmbossFilter* m_filter;
    float m_mix, m_stride, m_angle;
};

class HalftoneWidget : public QWidget
{
    Q_OBJECT
public:
    HalftoneWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_dotSize(0.0f)
    {
        m_ui.setupUi(this);
        initHalftone();
    }
    ~HalftoneWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initHalftone();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setHalftone(int n);

private:
    Ui::HalftoneWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEHalftoneFilter* m_filter;
    float m_dotSize;
};

class HazeWidget : public QWidget
{
    Q_OBJECT
public:
    HazeWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_distance(0.0f), m_slope(0.0f)
    {
        m_ui.setupUi(this);
        initHaze();
    }
    ~HazeWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initHaze();
    bool checkFilter();
    void _useColor();

protected slots:

    void preview();
    void apply();
    void setDistance(int n);
    void setSlope(int n);

private:
    Ui::HazeWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEHazeFilter* m_filter;
    float m_distance, m_slope, m_red, m_green, m_blue;
};

class PolkadotWidget : public QWidget
{
    Q_OBJECT
public:
    PolkadotWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_dotScaling(0.0f)
    {
        m_ui.setupUi(this);
        initPolkadot();
    }
    ~PolkadotWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initPolkadot();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setDotScaling(int n);

private:
    Ui::PolkadotWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEPolkaDotFilter* m_filter;
    float m_dotScaling;
};

class BilateralBlurWidget : public QWidget
{
    Q_OBJECT
public:
    BilateralBlurWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent),
        m_mainWindow(mainWindow),
        m_filter(nullptr),
        m_blurScale(0.0f),
        m_disFactor(0.0f),
        m_repeatTimes(1)
    {
        m_ui.setupUi(this);
        initBlateralBlur();
    }
    ~BilateralBlurWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initBlateralBlur();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setBlurScale(int n);
    void setDistanceFactor(int n);
    void setRepeatTimes(int n);

private:
    Ui::BilateralBlurWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEBilateralWrapperFilter* m_filter;
    float m_blurScale, m_disFactor, m_repeatTimes;
};

class LerpblurWidget : public QWidget
{
    Q_OBJECT
public:
    LerpblurWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f)
    {
        m_ui.setupUi(this);
        initLerpblur();
    }
    ~LerpblurWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initLerpblur();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setBlurLevel(int n);
    void setBlurBase(int base);

private:
    Ui::LerpBlurWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGELerpblurFilter* m_filter;
    float m_intensity, m_blurBase;
};

class SketchWidget : public QWidget
{
    Q_OBJECT
public:
    SketchWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_intensity(0.0f)
    {
        m_ui.setupUi(this);
        initSketch();
    }
    ~SketchWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initSketch();
    bool checkFilter();

protected slots:

    void preview();
    void apply();
    void setIntensity(int n);

private:
    Ui::SketchWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGESketchFilter* m_filter;
    float m_intensity;
};

class MaxValueWidget : public QWidget
{
    Q_OBJECT
public:
    MaxValueWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr)
    {
        m_ui.setupUi(this);
        initMaxValue();
    }
    ~MaxValueWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initMaxValue();
    bool checkFilter();

protected slots:

    void preview();
    void apply();

private:
    Ui::previewApplyWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEMaxValueFilter3x3* m_filter;
};

class MinValueWidget : public QWidget
{
    Q_OBJECT
public:
    MinValueWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr)
    {
        m_ui.setupUi(this);
        initMinValue();
    }
    ~MinValueWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initMinValue();
    bool checkFilter();

protected slots:

    void preview();
    void apply();

private:
    Ui::previewApplyWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEMinValueFilter3x3* m_filter;
};

class WaveWidget : public QWidget
{
    Q_OBJECT
public:
    WaveWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr), m_motion(0.0f), m_angle(0.0f), m_strength(0.0f)
    {
        m_ui.setupUi(this);
        initWave();
    }
    ~WaveWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initWave();
    bool checkFilter();

protected slots:

    void preview();
    void apply();

    void setMotion(int n);
    void setAngle(int n);
    void setStrength(int n);

private:
    Ui::WaveAdjustWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEDynamicWaveFilter* m_filter;
    float m_motion;
    float m_angle;
    float m_strength;
};

class ColorMappingWidget : public QWidget
{
    Q_OBJECT
public:
    ColorMappingWidget(MainWindow* mainWindow, QWidget* parent = nullptr) :
        QWidget(parent), m_mainWindow(mainWindow), m_filter(nullptr)
    {
        m_ui.setupUi(this);
        initColorMapping();
    }
    ~ColorMappingWidget()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
    }

protected:
    void initColorMapping();
    bool checkFilter();

protected slots:

    void setCurrentDir();
    void preview();
    void apply();

private:
    Ui::ColorMappingWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEColorMappingFilter* m_filter;
    int m_width, m_height;
    int m_unitWidth, m_unitHeight;
    QString m_filename;
};

//////////////////////////////////////////////////////////////////////////

class StyleAdjustWindow : public MenuDialogCommon
{
    Q_OBJECT
public:
    StyleAdjustWindow(MainWindow* mainwindow, QWidget* parent = nullptr);
    ~StyleAdjustWindow()
    {
        CGE_LOG_INFO("StyleAdjustWindow closed!\n");
        m_mainWindow->enableMenu(true);
    }

public slots:
    void switchItem(int index);

private:
    Ui::StyleAdjustWidget m_ui;
    MainWindow* m_mainWindow;
    QWidget* m_activeAdjustWidget;
};

#endif
