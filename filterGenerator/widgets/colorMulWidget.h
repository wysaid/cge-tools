/*
 * colorMulWidget.h
 *
 *  Created on: 2014-12-12
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _COLOR_MUL_WIDGET_
#define _COLOR_MUL_WIDGET_

#include "mainwindow.h"
#include "ui_colorMulWidget.h"
#include "cgeMultipleEffectsCommon.h"

class MainWindow;
class MenuDialogCommon;

class ColorMulWindow : public MenuDialogCommon
{
    Q_OBJECT
public:
    ColorMulWindow(MainWindow* mainWindow, QWidget* parent = nullptr);
    ~ColorMulWindow();

protected:
    bool checkFilter();
    void initWidgets();
    bool _preview(FormatContent* pContent);
    bool _previewVec(FormatContent* pContent);
    bool _previewMat(FormatContent* pContent);

protected slots:
    void preview();
    void apply();

private:
    Ui::colorMulWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEColorMulFilter* m_filter;
    CGE::CGEColorMulFilter::MulMode m_mode;
    float m_matValues[9], m_vecValues[3];
};

#endif