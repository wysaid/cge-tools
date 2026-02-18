/*
* resultWidget.h
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifndef _RESULT_WIDGET_H_
#define _RESULT_WIDGET_H_

#include "mainwindow.h"
#include "ui_resultDock.h"

class QWidget;
class MainWindow;

struct FilterResultFormat
{
	FilterResultFormat() : filter(nullptr) {}
	FilterResultFormat(const FormatContent& content, CGE::CGEImageFilterInterfaceAbstract* f) : formatContent(content), filter(f) {}	

	FormatContent formatContent;
	CGE::CGEImageFilterInterfaceAbstract* filter;
};

class ResultWidget : public QWidget
{
	Q_OBJECT
public:
    ResultWidget(MainWindow* mainWindow, QWidget* parent = nullptr) : QWidget(parent), m_mainWindow(mainWindow), m_isHover(false)
	{
		m_ui.setupUi(this);
		connect(m_ui.dockWidget, SIGNAL(visibilityChanged(bool)), SLOT(shouldClose(bool)));
	}
	~ResultWidget() {}

	void setHTML(const QString& html);

	bool isHover() { return m_isHover; }

	static inline bool sortComp(const ResultWidget* left, const ResultWidget* right)
	{
		return left->y() < right->y();
	}

	FilterResultFormat& getResult() { return m_result; }
	void setResult(FilterResultFormat result) { m_result = result; }

protected slots:

	void shouldClose(bool );

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void closeEvent(QCloseEvent *);

protected:
	MainWindow* m_mainWindow;
	Ui::ResultInfoWidget m_ui;

	FilterResultFormat m_result;

	int m_lastY;
	bool m_isHover;
};


#endif
