/*
 * textureBlendWidget.h
 *
 *  Created on: 2014-11-19
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _TEXTURE_BLEND_WIDGET_
#define _TEXTURE_BLEND_WIDGET_

#include "mainwindow.h"
#include "ui_textureBlendWidget.h"

// CGE filter headers
#include "cgeBlendFilter.h"
#include "cgeMultipleEffects.h"

class MainWindow;
class QWidget;
class MenuDialogCommon;

class TextureBlendWindow : public MenuDialogCommon
{
    Q_OBJECT
public:
    TextureBlendWindow(MainWindow* mainWindow, QWidget* parent = nullptr);
    ~TextureBlendWindow()
    {
        if (m_wrapFilter)
        {
            m_mainWindow->removeFilter(m_wrapFilter);
        }
        CGE_LOG_INFO("TextureBlendWindow closed!\n");
        m_mainWindow->enableMenu(true);
    }

    enum MapFunction
    {
        Map_None,
        Map_KeepRatio,
        Map_Tile,
    };

protected:
    bool checkFilter();
    void initWidgets();
    bool resetFilter(CGETextureBlendMode, MapFunction);
    bool loadTexture(const QString& filename);
    bool _preview();

protected slots:
    void preview();
    void apply();
    void chooseResourceFolder();
    void setBlendMode(int index);
    void setMapMode(int index);

private:
    Ui::TextureBlendWidget m_ui;
    MainWindow* m_mainWindow;
    CGE::CGEMutipleEffectFilter* m_wrapFilter;
    CGE::CGEBlendWithResourceFilter* m_filter;
    CGETextureBlendMode m_blendMode;
    QString m_texFileName;
    GLuint m_texture;
    CGE::CGESizei m_texSize;
    float m_intensity;

    MapFunction m_mapFunc;
};


#endif