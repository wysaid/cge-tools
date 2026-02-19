/*
 * toolWidget.h
 *
 *  Created on: 2015-5-14
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _TOOL_WIDGET_H_
#define _TOOL_WIDGET_H_

#include <QtWidgets/QSlider>
#include <functional>
#include <vector>

class ToolSliderWidget : public QSlider
{
    Q_OBJECT
public:
    ToolSliderWidget(QWidget* parent);
    ~ToolSliderWidget();

    void setFunc(const std::function<void(int)>& func);

protected:
    void focusOutEvent(QFocusEvent*);
    void keyPressEvent(QKeyEvent* ev);

public slots:

    void setSliderValue(int);

protected:
    std::function<void(int)> m_func;
    static std::vector<ToolSliderWidget*> s_toolWidgets;
};

#endif  // !_TOOL_WIDGET_H_
