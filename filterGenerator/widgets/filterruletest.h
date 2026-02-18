/*
* filterRuleTest.h
*
*  Created on: 2014-11-26
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/


#ifndef FILTERRULETEST_H
#define FILTERRULETEST_H

#include "cge.h"
#include <QtWidgets/QMainWindow>
#include "ui_filterruletest.h"
#include "canvasWidget.h"

class FilterRuleTest : public QMainWindow
{
	Q_OBJECT

public:
	FilterRuleTest(QWidget *parent = 0);
	~FilterRuleTest();

    CanvasWidget* getCanvas() { return m_canvasWidget; }

protected:
	void initWidgets();
    void resizeEvent(QResizeEvent*);


protected slots:
	void testRule();
	void saveResult();
	void batchEffect();
	void chooseInputImage();
	void chooseResourceFolder();


private:
	Ui::FilterRuleTestClass m_ui;
	CanvasWidget* m_canvasWidget;
};

#endif // FILTERRULETEST_H
