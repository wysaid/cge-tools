/*
 * menuWidget.h
 *
 *  Created on: 2014-11-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#pragma once

#include "mainwindow.h"

class MainWindow;
class MenuWidget;

enum MenuItems
{
    Basic_Adjusts,
    Curve_Adjust,
    Texture_Blend,
    Color_Blend,
    Color_Multiply,
    Advanced_Adjusts,
    Face_Effect_Tool,
    // 	Color_Balance,
    // 	Tone_Mapping
};

class MenuItemWidget : public QPushButton
{
    Q_OBJECT
public:
    MenuItemWidget(MenuWidget* menuWidget, MenuItems index, QWidget* parent = nullptr);

public slots:

    void itemClicked();

private:
    MenuItems m_itemIndex;
    MenuWidget* m_menuWidget;
};

class MenuWidget : public QWidget
{
public:
    MenuWidget(MainWindow* mainWindow, QWidget* parent = nullptr);

    void runProc(MenuItems index);

protected:
    void resizeEvent(QResizeEvent* e);

protected:
    QScrollArea* m_scrollArea;
    QWidget* m_childWidget;  //, *m_popupWidget;
    MainWindow* m_mainWindow;
};
