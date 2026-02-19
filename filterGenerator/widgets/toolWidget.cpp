/*
 * toolWidget.cpp
 *
 *  Created on: 2015-5-14
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#include "toolWidget.h"
#include <QKeyEvent>

std::vector<ToolSliderWidget*> ToolSliderWidget::s_toolWidgets;

ToolSliderWidget::ToolSliderWidget(QWidget* parent) : QSlider(parent), m_func(nullptr)
{
    for (auto w : s_toolWidgets)
    {
        w->deleteLater();
    }

    s_toolWidgets.clear();

    s_toolWidgets.push_back(this);

    connect(this, SIGNAL(valueChanged(int)), SLOT(setSliderValue(int)));
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setOrientation(Qt::Horizontal);
    setFixedSize(300, 30);
    setStyleSheet("background-color:#009; color:#fff;");
}

ToolSliderWidget::~ToolSliderWidget()
{
    for (auto&& iter = s_toolWidgets.begin(); iter != s_toolWidgets.end(); ++iter)
    {
        if (*iter == this)
        {
            s_toolWidgets.erase(iter);
            break;
        }
    }
}

void ToolSliderWidget::setSliderValue(int v)
{
    if (m_func != nullptr)
    {
        m_func(v);
    }
}

void ToolSliderWidget::setFunc(const std::function<void(int)>& func)
{
    m_func = func;
}

void ToolSliderWidget::focusOutEvent(QFocusEvent*)
{
    deleteLater();
}

void ToolSliderWidget::keyPressEvent(QKeyEvent* ev)
{
    if (ev->key() == Qt::Key_Escape) deleteLater();
}