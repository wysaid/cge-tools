/*
 * faceEffectTool.h
 *
 *  Created on: 2017-4-12
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _FACE_EFFECT_TOOL_H_

#include "mainwindow.h"
#include <QTimer>
#include <QTime>
#include <functional>
#include "ui_faceEffectTool.h"
#include "../../cge/src/sprite/cgeSprite2d.h"

#include "../../cge/src/core/cgeGeometryUtils.h"

#include <QJsonDocument>

#define FACE_SPRITE_MULTIPLE 0

class FaceEffectToolWindow;

class FaceSpriteWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    FaceSpriteWidget(QWidget* parent, QOpenGLWidget* shareWidget);
    ~FaceSpriteWidget();

    enum
    {
        MAX_POINTS = FACE_SPRITE_MULTIPLE ? 16 : 2
    };

    struct FacePoints
    {
        CGE::Vec2f pnts[MAX_POINTS];
    };

    float getProgress();
    int getCurrentFrame();

    void setupWithJsonConfig(const QString& jsonConfig, const std::function<void(bool)>& callback);

    bool isPlaying();

    CGE::Sprite2dInterChangeMultiple* sprite() { return m_sprite; }

    QSize spriteSize() { return m_spriteSize; }

    void queueEvent(std::function<void()>&& func);

    CGE::CGEPointDrawer* getPointDrawer() { return m_pointDrawer; }

    QString genConfig();

    const std::vector<FacePoints>& getPoints() const { return m_vecPoints; }
    void setPoints(const std::vector<FacePoints>& pnts)
    {
        assert(pnts.size() == m_vecPoints.size());
        m_vecPoints = pnts;
        _calcDrawPoints();
    }

public slots:
    void startPlay();
    void pausePlay();
    void jumpFrame(float progress);
    void updateProgress();

signals:
    void progressUpdated();

protected:
    void paintGL();
    void resizeGL(int w, int h);

    void _calcDrawPoints();

protected:
    CGE::Sprite2dInterChangeMultiple* m_sprite;
    QTimer m_timer;
    QTime m_lastTime;
    QSize m_spriteTextureSize;
    QSize m_spriteSize;
    std::list<std::function<void()>> m_dispatchFunctions;

    std::vector<FacePoints> m_vecPoints;
    std::vector<FacePoints> m_vecDrawPoints;
    CGE::CGEPointDrawer* m_pointDrawer;

    QJsonDocument m_jsonDoc;
};

// class FaceEffectToolMarkWidgetCanvas : public QGLWidget
// {
// public:
// 	FaceEffectToolMarkWidgetCanvas(QWidget* parent, FaceSpriteWidget* shareWidget);
//
// protected:
// 	void paintGL();
// 	void resizeGL(int w, int h);
//
//
// protected:
// 	FaceSpriteWidget* m_spriteWidget;
// };

class FaceEffectToolMarkWidget : public MenuDialogCommon
{
public:
    FaceEffectToolMarkWidget(FaceEffectToolWindow* parent,
                             const std::function<void(std::vector<FaceSpriteWidget::FacePoints>*)>& func);
    ~FaceEffectToolMarkWidget();

protected:
    //	void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent*);
    void keyReleaseEvent(QKeyEvent*);

    void flushCache();

    void mouseReleaseEvent(QMouseEvent*);

protected:
    std::function<void(std::vector<FaceSpriteWidget::FacePoints>*)> m_callback;

    //	FaceEffectToolMarkWidgetCanvas* m_canvas;
    FaceEffectToolWindow* m_parent;

    QImage m_cacheImage;
    CGE::FrameBuffer* m_framebuffer;
    GLuint m_cacheTexture;

    std::vector<FaceSpriteWidget::FacePoints> m_vecPoints;

    int m_currentTarget;
};

class FaceEffectToolWindow : public QWidget
{
    Q_OBJECT
public:
    FaceEffectToolWindow(const std::function<void()>& func, MainWindow* mainwindow);
    ~FaceEffectToolWindow();

    FaceSpriteWidget* spriteWidget() { return m_spriteWidget; }

protected slots:
    void windowClose();
    void applyConfig();
    void saveConfig();
    void jumpToFixedFrame();
    void jumpToFrame(int frameIndex);

    void updateProgress();

    void setConfigDir();

    void textLeftBtnClicked();
    void textRightBtnClicked();

    void markFrame();

protected:
    void closeEvent(QCloseEvent*);
    void resizeEvent(QResizeEvent*);

    void _fixLayout();

    void _moveUI(int n);

protected:
    MainWindow* m_mainWindow;
    FaceSpriteWidget* m_spriteWidget;
    Ui::FaceEffectToolWidget m_ui;
    std::function<void()> m_destructFunc;
};

#endif  // !_FACE_EFFECT_TOOL_H_
