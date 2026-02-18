/*
* colorBlendWidget.h
*
*  Created on: 2014-11-21
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifndef _PIXEL_BLEND_WIDGET_
#define _PIXEL_BLEND_WIDGET_

#include "mainwindow.h"
#include "ui_colorBlendWidget.h"

// CGE filter headers
#include "cgeBlendFilter.h"
#include "cgeMultipleEffects.h"

class MainWindow;
class MenuDialogCommon;

class ColorBlendWindow : public MenuDialogCommon
{
	Q_OBJECT
public:
	ColorBlendWindow(MainWindow* mainWindow, QWidget* parent = nullptr);
	~ColorBlendWindow();

	enum ColorBlendMode
	{
		ColorBlend_Pixel,
		ColorBlend_Vignette,
		ColorBlend_Self,
	};

protected:
	void initWidgets();

	bool _previewBlendPixel();
	bool _previewBlendVignette();
	bool _previewBlendSelf();

	bool _applyBlendPixel();
	bool _applyBlendVignette();
	bool _applyBlendSelf();

	bool _preview();

	bool checkFilter();

protected slots:
	void preview();
	void apply();

	void setVignetteStart(int);
	void setVignetteRange(int);
	void switchBlendMode(int);


private:
	Ui::ColorBlendWidget m_ui;
	MainWindow* m_mainWindow;
	CGE::CGEMutipleEffectFilter* m_wrapFilter;
	CGE::CGEBlendInterface* m_filter;
	CGE::CGEPixblendFilter* m_filterPixelBlend;
	CGE::CGEBlendVignetteFilter* m_filterVignetteBlend;
	CGE::CGEBlendWithSelfFilter* m_filterSelfBlend;
	CGETextureBlendMode m_texBlendMode;
	ColorBlendMode m_colorBlendMode;

	float m_vigStart, m_vigRange;

	bool m_bVigBlendAlpha;
};


#endif