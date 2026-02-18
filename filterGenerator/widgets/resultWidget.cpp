/*
* resultWidget.cpp
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "resultWidget.h"

void ResultWidget::setHTML(const QString& html)
{
	m_ui.textWidget->setHtml(html);
}

void ResultWidget::mousePressEvent(QMouseEvent *e)
{
	m_ui.dockWidget->setCursor(Qt::CursorShape::ClosedHandCursor);
	raise();
	m_lastY = e->globalY();
	m_isHover = true;
	CGE_LOG_INFO("Result Widget Hover\n");
}

void ResultWidget::mouseReleaseEvent(QMouseEvent *)
{
    if(!m_isHover)
        return;

	m_ui.dockWidget->setCursor(Qt::CursorShape::OpenHandCursor);
	m_isHover = false;
	m_mainWindow->sortSteps();
	m_mainWindow->sortFilters();
	update();
	CGE_LOG_INFO("Result Widget Hover Release\n");
}

void ResultWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(m_isHover)
	{
		move(x(), y() + e->globalY() - m_lastY);
		m_lastY = e->globalY();

// Compatibility: fix the issue of losing focus when the mouse moves outside the window on OS X
#ifdef Q_OS_OSX

		auto w = m_mainWindow->getOutputScrollWidget();		
		auto p = w->mapFromGlobal(e->globalPos());

        if(p.y() < -50)
        {
            m_ui.dockWidget->setCursor(Qt::CursorShape::ArrowCursor);
            m_ui.dockWidget->setFocus();
            m_isHover = false;
            m_mainWindow->sortFilters();
        }
#endif

		m_mainWindow->sortSteps();
		update();
	}
}

void ResultWidget::closeEvent(QCloseEvent *)
{
	m_mainWindow->removeStep(this);
}

void ResultWidget::shouldClose(bool visible)
{
	if(!visible)
		close();
}
