/*
* menuWidget.cpp
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "menuWidget.h"
#include "basicAdjustWidget.h"
#include "curveAdjustWidget.h"
#include "textureBlendWidget.h"
#include "colorBlendWidget.h"
#include "colorMulWidget.h"
#include "styleAdjustWidget.h"
#include "FaceEffectToolWindow.h"

static CGEConstString s_menuItems[] = {
//	"BasicAdjust",	//0
//	"CurveAdjust",	//1
//	"TextureBlend",	//2
//	"ColorBlend", //3
//	"ColorMultiply", //4
//	"AdvancedAdjust", //5
    "BasicAdjust",	//0
    "CurveAdjust",	//1
    "TextureBlend",	//2
    "ColorBlend", //3
    "ColorMultiply", //4
    "AdvancedAdjust", //5
	"FaceEffectTool", //6
};

static const int s_menuItemNum = sizeof(s_menuItems) / sizeof(*s_menuItems);

MenuItemWidget::MenuItemWidget(MenuWidget* menuWidget, MenuItems index, QWidget* parent) : QPushButton(parent)
{
	m_itemIndex = index;
	m_menuWidget = menuWidget;
	connect(this, SIGNAL(clicked()), SLOT(itemClicked()));
}

void MenuItemWidget::itemClicked()
{
	if(m_menuWidget)
		m_menuWidget->runProc(m_itemIndex);
}

MenuWidget::MenuWidget(MainWindow* mainWindow, QWidget* parent) : QWidget(parent), m_mainWindow(mainWindow)//, m_popupWidget(nullptr)
{
	m_scrollArea = new QScrollArea(this);
	m_childWidget = new QWidget(m_scrollArea);

	const int buttonSize = 30;

	m_childWidget->setGeometry(0, 0, 150, (buttonSize + 5) * s_menuItemNum);
	m_scrollArea->setGeometry(0, 0, width(), height());

	for(int i = 0; i != s_menuItemNum; ++i)
	{
		auto btn = new MenuItemWidget(this, (MenuItems)i, m_childWidget);
		btn->setText(QString::fromLocal8Bit(s_menuItems[i]));
		btn->setGeometry(5, i * (buttonSize + 5), 140, buttonSize);
	}
	
	m_scrollArea->setWidget(m_childWidget);
}

void MenuWidget::runProc(MenuItems index)
{
	CGE_LOG_INFO("%s\n", s_menuItems[index]);

	switch (index)
	{
	case Basic_Adjusts:
		{
			auto w = new BasicAdjustWindow(m_mainWindow, m_mainWindow);
			w->show();
		}
		break;
	case Curve_Adjust:
		{
			auto w = new CurveAdjustWindow(m_mainWindow, m_mainWindow);
			w->show();
		}
		break;
	case Texture_Blend:
		{
			auto w = new TextureBlendWindow(m_mainWindow, m_mainWindow);
			w->show();
		}
		break;
	case Color_Blend:
		{
			auto w = new ColorBlendWindow(m_mainWindow, m_mainWindow);
			w->show();
		}
		break;
	case Color_Multiply:
		{
			auto w = new ColorMulWindow(m_mainWindow, m_mainWindow);
			w->show();
		}
		break;
	case Advanced_Adjusts:
		{
			auto w = new StyleAdjustWindow(m_mainWindow, m_mainWindow);
			w->show();
		}
		break;
	case Face_Effect_Tool:
		{
			m_mainWindow->hide();
			auto w = new FaceEffectToolWindow([&](){
				m_mainWindow->show();
			}, m_mainWindow);
			w->show();
		}
		break;
	default:
		break;
	}
}

void MenuWidget::resizeEvent(QResizeEvent* e)
{
	m_scrollArea->resize(size());
}
