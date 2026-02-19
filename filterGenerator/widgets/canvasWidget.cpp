/*
 * canvasWidget.cpp
 *
 *  Created on: 2014-11-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#include <QMessageBox>
#include <QMouseEvent>
#include <QMimeData>
#include "canvasWidget.h"

#define CLOCK_TRANSLATE (CLOCKS_PER_SEC / 1000)

CGE::SharedTexture genSharedTextureWidthImageName(const char* filename, GLenum filter)
{
    QImage&& img = QImage(filename).convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly);
    if (img.width() < 1)
    {
        CGE_LOG_ERROR("Invalid image!\n");
        return CGE::SharedTexture(0, 0, 0);
    }
    GLuint texID =
        cgeGenTextureWithBuffer(img.bits(), img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE, 4, 0, filter);
    return CGE::SharedTexture(texID, img.width(), img.height());
}

//////////////////////////////////////////////////////////////////////////

// Checkerboard grid size in pixels (each square is GRID_SIZE x GRID_SIZE)
static const float s_checkerboardGridSize = 8.0;

static CGEConstString s_vshBackGround = CGE_SHADER_STRING(

    attribute vec4 position; varying vec2 texCoord;

    uniform vec2 canvasSize;

    void main() {
        gl_Position = position;
        texCoord = vec2(position.x + 1.0, 1.0 - position.y) / 2.0 * canvasSize;
    });

// Photoshop-style transparency checkerboard: white (#FFFFFF) and light gray (#CCCCCC)
static CGEConstString s_fshBackGround = CGE_SHADER_STRING_PRECISION_H(

    varying vec2 texCoord; uniform float gridSize;

    void main() {
        vec2 cell = floor(texCoord / gridSize);
        float checker = mod(cell.x + cell.y, 2.0);
        float gray = mix(1.0, 0.8, checker);
        gl_FragColor = vec4(gray, gray, gray, 1.0);
    });


//////////////////////////////////////////////////////////////////////////

CanvasWidget::CanvasWidget(QWidget* parent) :
    QOpenGLWidget(parent), m_isMoving(false), m_program(nullptr), m_intensity(1.0f)
{
    setAttribute(Qt::WA_NoSystemBackground);
    setCursor(Qt::CursorShape::OpenHandCursor);
    setFocusPolicy(Qt::ClickFocus);
    setAcceptDrops(true);
}

CanvasWidget::~CanvasWidget()
{}

void CanvasWidget::paintGL()
{
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());

    if (m_program != nullptr)
    {
        glDisable(GL_BLEND);
        m_program->bind();

        glBindBuffer(GL_ARRAY_BUFFER, CGE::CGEGlobalConfig::sVertexBufferCommon);
        glEnableVertexAttribArray(m_posAttribLocation);
        glVertexAttribPointer(m_posAttribLocation, 2, GL_FLOAT, false, 0, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if (m_sprite)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_sprite->render();
    }
    cgeCheckGLError("paintGL");
}

void CanvasWidget::initializeGL()
{
#ifdef CGE_USE_COMPATIBLE_GL_FUNCTIONS
    g_glFunctions = this->context()->functions();
#endif

    cgePrintGLInfo();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CGE::SpriteCommonSettings::sFlipCanvas(false, true);
    cgeInitialize(width(), height(), CGE::CGEGlobalConfig::InitArguments(CGE::CGEGlobalConfig::CGE_INIT_SPRITEBUILTIN));
    // 	CGE::cgeSetCommonLoadFunction(resourceLoadFunc, nullptr);
    // 	CGE::cgeSetCommonUnloadFunction(resourceReleaseFunc, nullptr);
    //	CGE::cgeInitExtends();

    CGE::SharedTexture t = genSharedTextureWidthImageName(":CGE/resource/Frieren.jpg");

    m_handler.initWithTexture(t.texID(), t.width, t.height, CGE_FORMAT_RGBA_INT8, true);

    m_sprite = new DisplaySprite(t);
    m_sprite->moveTo(300, 300);

    m_program = new CGE::ProgramObject;
    m_posAttribLocation = 0;
    m_program->bindAttribLocation("position", m_posAttribLocation);
    if (m_program->initWithShaderStrings(s_vshBackGround, s_fshBackGround))
    {
        m_program->bind();
        m_program->sendUniformf("canvasSize", (float)width(), (float)height());
        m_program->sendUniformf("gridSize", s_checkerboardGridSize);
    }
    else
    {
        delete m_program;
        m_program = nullptr;
        CGE_LOG_ERROR("Failed to initialize background, will display background as black");
    }

    fitImage();
}

// Compatible with Retina display: can use w and h converted by deviceRatio
void CanvasWidget::resizeGL(int w, int h)
{
    m_sprite->setCanvasSize(width(), height());
    if (m_program != nullptr)
    {
        m_program->bind();
        m_program->sendUniformf("canvasSize", (float)width(), (float)height());
    }
}

void CanvasWidget::mousePressEvent(QMouseEvent* e)
{
    setCursor(Qt::CursorShape::ClosedHandCursor);
    m_isMoving = true;
    m_lastX = (int)e->globalPosition().x();
    m_lastY = (int)e->globalPosition().y();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (m_isMoving)
    {
        auto px = (int)e->globalPosition().x(), py = (int)e->globalPosition().y();
        m_sprite->move(px - m_lastX, py - m_lastY);
        m_lastX = px;
        m_lastY = py;
        update();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* e)
{
    setCursor(Qt::CursorShape::OpenHandCursor);
    m_isMoving = false;
}

void CanvasWidget::wheelEvent(QWheelEvent* e)
{
    auto scaling = 1.0f + e->angleDelta().y() / 2000.0f;

    imageZoom(scaling);

    auto s = m_sprite->getScaling();

    s[0] = CGE::CGE_MID(s[0], 0.05f, 5.0f);
    s[1] = CGE::CGE_MID(s[1], 0.05f, 5.0f);

    m_sprite->scaleTo(s[0], s[1]);
    update();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key())
    {
        case Qt::Key_Space:
            fitImage();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            restoreImage();
            break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            setIntensity(m_intensity += 0.1f);
            restoreImage();
            m_handler.revertToKeptResult(false);
            m_handler.processingFilters();
            setToCurrentImage();
            break;
        case Qt::Key_Minus:
        case Qt::Key_Underscore:
            setIntensity(m_intensity -= 0.1f);
            restoreImage();
            m_handler.revertToKeptResult(false);
            m_handler.processingFilters();
            setToCurrentImage();
            break;
        case Qt::Key_0:
            m_intensity = 0.0f;
            break;
        case Qt::Key_1:
            m_intensity = 1.0f;
            break;
        default:
            break;
    }
}

void CanvasWidget::dragEnterEvent(QDragEnterEvent* e)
{
    auto mimedata = e->mimeData();
    if (mimedata->hasImage() || mimedata->hasText())
    {
        e->acceptProposedAction();
    }
}

void CanvasWidget::dropEvent(QDropEvent* e)
{
    auto names = e->mimeData()->urls();
    if (names.empty()) return;
    auto url = names.first();
    auto filename = url.toLocalFile();
    if (filename.isEmpty())
    {
        QMessageBox::warning(this, "Invalid Image!", QString("%1 is not a local image file!").arg(url.toString()));
        return;
    }
    openImage(filename);
}

bool CanvasWidget::openImage(const QString& filename)
{
    CGE_LOG_INFO("Opening image %s ...\n", (const char*)filename.toLocal8Bit());
    QImage img(filename);
    if (img.isNull())
    {
        QMessageBox::information(
            this, "Cannot Open Image",
            QString(
                "%1 cannot be recognized as a valid supported image format and cannot be opened. Please select another file!")
                .arg(filename));
        return false;
    }
    return openImage(img);
}

bool CanvasWidget::openImage(QImage& img)
{

#ifdef Q_OS_WIN
    const float sMaxImageSize = 30000000.0f;
#else
    const float sMaxImageSize = 5000000.0f;
#endif

    if (img.width() * img.height() > sMaxImageSize)
    {
        CGE_LOG_ERROR(
            "%s",
            (const char*)QString(
                "The selected image size is %1 * %2, totaling %3M pixels. It is too large for real-time processing. To prevent excessive memory usage and maintain adjustment speed, the image will be cropped to within %4M pixels.\n")
                .arg(img.width())
                .arg(img.height())
                .arg(img.width() * img.height() / 1e6f)
                .arg(sMaxImageSize / 1e6f)
                .toLocal8Bit());
        float ratio = sqrtf(sMaxImageSize / (img.width() * img.height()));
        QSize s(img.width() * ratio, img.height() * ratio);
        img = img.scaled(s, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    if (img.format() != QImage::Format_RGBA8888) img = img.convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly);

    CGE_LOG_INFO("Image opened successfully...\n");
    makeCurrent();
    if (m_handler.initWithRawBufferData(img.bits(), img.width(), img.height(), CGE_FORMAT_RGBA_INT8, true))
    {
        if (m_handler.getFilterNum() != 0) m_handler.processingFilters();
        auto sz = m_handler.getOutputFBOSize();
        m_sprite->assignTexture(m_handler.getTargetTextureID(), sz.width, sz.height);
        update();
        return true;
    }
    else
    {
        QMessageBox::information(this, "Unknown Error", "Failed to open image");
        return false;
    }
}

void CanvasWidget::setToOriginImage()
{
    if (m_sprite != nullptr)
    {
        makeCurrent();
        auto sz = m_handler.getOutputFBOSize();
        m_sprite->assignTexture(m_handler.getSourceTextureID(), sz.width, sz.height);
        update();
    }
    CGE_LOG_INFO("setToOriginImage\n");
}

void CanvasWidget::setToCurrentImage()
{
    if (m_sprite != nullptr)
    {
        makeCurrent();
        auto sz = m_handler.getOutputFBOSize();
        m_sprite->assignTexture(m_handler.getTargetTextureID(), sz.width, sz.height);
        update();
    }
    CGE_LOG_INFO("setToCurrentImage\n");
}

void CanvasWidget::restoreImage()
{
    m_sprite->moveTo(width() / 2.0f, height() / 2.0f);
    m_sprite->scaleTo(1.0f, 1.0f);
    update();
    CGE_LOG_INFO("restoreImage\n");
}

void CanvasWidget::fitImage()
{
    auto& tex = m_sprite->getTexture();
    auto scaling = CGE::CGE_MIN((float)width() / tex.width, (float)height() / tex.height);
    m_sprite->scaleTo(scaling, scaling);
    m_sprite->moveTo(width() / 2.0f, height() / 2.0f);
    update();
}

void CanvasWidget::imageZoom(float scaling)
{
    m_sprite->scale(scaling, scaling);
}

void CanvasWidget::setIntensity(float value)
{
    auto& filters = m_handler.peekFilters();

    for (auto filter : filters)
    {
        filter->setIntensity(value);
    }
}

void CanvasWidget::imageZoomIn()
{
    imageZoom(0.9f);
    update();
}

void CanvasWidget::imageZoomOut()
{
    imageZoom(1.1f);
    update();
}
