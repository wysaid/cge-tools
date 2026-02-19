/*
 * curveAdjustWidget.h
 *
 *  Created on: 2014-11-11
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CURVE_ADJUST_WIDGET_
#define _CURVE_ADJUST_WIDGET_

#include "mainwindow.h"
#include "ui_curveAdjustWidget.h"
#include "cgeFilterBasic.h"
#include "cgeCurveAdjust.h"

class MainWindow;
class QWidget;
class MenuDialogCommon;
class CurveAdjustWindow;

class SingleCurveWidget : public QWidget
{
public:
    enum CurveChannel
    {
        Curve_R = Qt::red,
        Curve_G = Qt::green,
        Curve_B = Qt::blue,
        Curve_RGB = Qt::white
    };

    SingleCurveWidget(CurveAdjustWindow* curveAdjustWindow, QLineEdit* edit, CurveChannel channel,
                      QWidget* parent = nullptr);
    ~SingleCurveWidget() {}

    inline QImage& getImage() { return m_image; }

    void drawCurveImage(std::vector<CGE::CGECurveInterface::CurvePoint>&);
    void drawCurveImage() { drawCurveImage(m_curvePoints); }

    void validateText(std::vector<CGE::CGECurveInterface::CurvePoint>* p = nullptr);

    std::vector<CGE::CGECurveInterface::CurvePoint>& getCurvePoints() { return m_curvePoints; }
    std::vector<float>& getCurveData() { return m_curveData; }
    QString getCurveText() { return m_curveEdit->text(); }

    void setCurvePoints(const std::vector<CGE::CGECurveInterface::CurvePoint>& vecPoints) { m_curvePoints = vecPoints; }

    void resetWidgetCurve();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

private:
    QImage m_image;
    std::vector<CGE::CGECurveInterface::CurvePoint> m_curvePoints, m_cachePoints;
    std::vector<float> m_curveData;
    CurveChannel m_channel;
    CurveAdjustWindow* m_curveAdjustWindow;
    QLineEdit* m_curveEdit;
    bool m_isHover;
};

class CurveAdjustWindow : public MenuDialogCommon
{
    Q_OBJECT
public:
    CurveAdjustWindow(MainWindow* mainWindow, QWidget* parent = nullptr);
    ~CurveAdjustWindow()
    {
        if (m_filter)
        {
            m_mainWindow->removeFilter(m_filter);
        }
        CGE_LOG_INFO("CurveAdjustWindow closed!\n");
        m_mainWindow->enableMenu(true);
    }

    void updateFilter();

protected:
    void initWidgets();
    bool checkFilter();

    bool _isBrakeLeft(const char* separator);
    bool _isBrakeRight(const char* separator);
    std::vector<CGE::CGECurveInterface::CurvePoint> _genCurvePointWithData(const char* dataText);
    bool _isUsefulCurve(const std::vector<CGE::CGECurveInterface::CurvePoint>&) const;

protected slots:
    void preview();
    void apply();

private:
    Ui::CurveAdjustWidget m_ui;
    MainWindow* m_mainWindow;
    SingleCurveWidget *m_rgbWidget, *m_rWidget, *m_gWidget, *m_bWidget;
    CGE::CGEMoreCurveFilter* m_filter;
};


#endif
