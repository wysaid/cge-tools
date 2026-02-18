/*
* mainwindow.h
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#pragma once

#include <QImage>
#include <QPushButton>
#include <QtWidgets>
#include <QDialog>
#include <QBasicMutex>
#include "ui_mainwindow.h"
#include "ui_exportWidget.h"
#include "canvasWidget.h"
#include "cge.h"

struct FormatContent 
{
	QString cge;
	QString android;
	QString flash;
};

#include "menuWidget.h"
#include "resultWidget.h"

#define FILTER_COMMON_ERROR_MSG do { QMessageBox::information(this, "Invalid Parameters", "Some parameter values are invalid or not filled in! Please check."); } while(0)


// Only provides fixed-parameter methods. For variable-parameter methods such as curves or selective colors, please modify the corresponding code directly.
struct FilterFormats
{
	typedef struct Format
	{
		Format() {}
		Format(const QString& sCGE, const QString& sAndroid, const QString& sFlash) : cge(sCGE), android(sAndroid), flash(sFlash) {}
		QString cge;
		QString android;
		QString flash;
	}Format;

	static Format brightness;
	static Format contrast;
	static Format saturation;
	static Format exposure;
	static Format hue;
	static Format shadowHighlight;
	static Format blurSharpen;
	static Format vignette;
	static Format hsl;
	static Format hsv;
	static Format monochrome;
	static Format whiteBalance;
	static Format colorBalance;
	static Format colorLevel;

//	static Format selectiveColor;
	static Format texBlend;
	static Format texBlendKeepRatio;
	static Format tileBlend;

	static Format pixelBlend;
	static Format vignetteBlend;
	static Format vignetteBlendWithNoAlpha;
	static Format selfBlend;
	
	//////////////////////////////////////////////////////////////////////////
	static Format crossHatch;
	static Format edge;
	static Format emboss;
	static Format halftone;
	static Format haze;
	static Format polkadot;
	static Format bilateralBlur;
	static Format lerpBlur;
	static Format sketch;
	static Format max;
	static Format min;
	static Format mid;
    static Format wave;
	static Format lookup;
	static Format colorMapping;
	static Format colorMulVec;
	static Format colorMulMat;
};

class CanvasWidget;
class MainWindow;
class MenuWidget;
class ResultWidget;
class QDialog;

class MenuDialogCommon : public QDialog
{
public:
	MenuDialogCommon(QWidget* parent) : QDialog(parent)
	{
		setAttribute(Qt::WA_DeleteOnClose);
	}
	~MenuDialogCommon()
	{
		CGE_LOG_INFO("MenuDialogCommon closed\n");
	}

protected:
	virtual void applyAndQuit();
};

class ExportWindow : public MenuDialogCommon
{
	Q_OBJECT
public:
	ExportWindow(QWidget* parent = nullptr);

protected slots:
	void pathChooseBtnClicked();
	void applyBtnClicked();
	void cancelBtnClicked();

protected:
	void _saveAsCGE();
	void _saveAsAndroid();
	void _saveAsFlash();

private:
	Ui::ExportWidget m_ui;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void appendStep(CGE::CGEImageFilterInterfaceAbstract* filter, const FormatContent& content);

	bool removeStep(ResultWidget* step);

	void sortSteps();

	void sortFilters();

	void enableMenu(bool useMenu);

	void appendFilter(CGE::CGEImageFilterInterfaceAbstract* filter);

	void useCanvasContext();

	void runFilters();
	void disableRunFilters(bool b) { m_stopRunFilters = b; }

	void removeFilter(CGE::CGEImageFilterInterfaceAbstract* filter, bool bDel = true);

	void refreshOutputWidget();

	QWidget* getOutputWidget() { return m_ui.outputWidget; }
	QWidget* getOutputScrollWidget() { return m_ui.outputScrollWidget; }

	CanvasWidget* getCanvasWidget() { return m_canvasWidget; }

public slots:

	void openImage();
	void saveResults();
	void previewResults();
	void useLUT();

protected:
	void resizeEvent(QResizeEvent *);
	void closeEvent(QCloseEvent *);

protected:
	void _initWidgets();
	void _saveConfig(const QString&);
	void _saveImage(const QString&);
	void _saveString(const QString& filename, const QString& data);

private:
	Ui::UIMainWindow m_ui;
	CanvasWidget* m_canvasWidget;
	MenuWidget* m_menuWidget;
	
	std::vector<ResultWidget*> m_resultSteps;
	QMutex m_mutex;
	bool m_stopRunFilters;
	bool m_shouldSave;
};
