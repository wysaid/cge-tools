/*
 * curveAdjustWidget.cpp
 *
 *  Created on: 2014-11-11
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#include "curveAdjustWidget.h"
#include "../../cge/src/core/cgeGlobal.h"

SingleCurveWidget::SingleCurveWidget(CurveAdjustWindow* curveAdjustWindow, QLineEdit* edit, CurveChannel channel,
                                     QWidget* parent) :
    QWidget(parent), m_curveAdjustWindow(curveAdjustWindow), m_curveEdit(edit), m_isHover(false), m_channel(channel)
{
    setGeometry(0, 0, parent->width(), parent->height());
    m_image = QImage(size(), QImage::Format_RGB888);
    m_curvePoints.push_back(CGE::CGECurveInterface::makeCurvePoint(0.0f, 0.0f));
    m_curvePoints.push_back(CGE::CGECurveInterface::makeCurvePoint(1.0f, 1.0f));
    drawCurveImage(m_curvePoints);
    //	validateText();
    setCursor(Qt::CrossCursor);
}

void SingleCurveWidget::resetWidgetCurve()
{
    m_curvePoints.clear();
    m_curvePoints.push_back(CGE::CGECurveInterface::makeCurvePoint(0.0f, 0.0f));
    m_curvePoints.push_back(CGE::CGECurveInterface::makeCurvePoint(1.0f, 1.0f));
    drawCurveImage(m_curvePoints);
    m_curveEdit->setText("");
}

void SingleCurveWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawImage(rect(), m_image);
}

void SingleCurveWidget::drawCurveImage(std::vector<CGE::CGECurveInterface::CurvePoint>& vecPoints)
{
    using namespace CGE;

    if (vecPoints.size() <= 1) return;

    std::vector<float> curve(CGE_CURVE_PRECISION);
    CGECurveInterface::genCurve(m_curveData, vecPoints.data(), vecPoints.size());

    QPointF pLast(0.0f, m_image.height());
    float scalingX = m_image.width() / float(CGE_CURVE_PRECISION - 1);
    float scalingY = m_image.height();

    QPainter painter(&m_image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(m_image.rect(), Qt::black);
    painter.setPen((Qt::GlobalColor)m_channel);

    for (int i = 0; i < CGE_CURVE_PRECISION; ++i)
    {
        QPointF pCurrent(i * scalingX, (1.0f - m_curveData[i]) * scalingY);
        painter.drawLine(pCurrent, pLast);
        pLast = pCurrent;
    }

    char buffer[128];
    std::for_each(vecPoints.begin(), vecPoints.end(), [&](decltype(vecPoints[0]) cp) {
        sprintf(buffer, "(%d, %d)", int(cp.x * 255.0f), int(cp.y * 255.0f));
        QPointF p(m_image.width() * cp.x, m_image.height() * (1.0f - cp.y));
        painter.drawEllipse(p, 3, 3);
        painter.drawText(p, buffer);
    });

    update();
}


void SingleCurveWidget::validateText(std::vector<CGE::CGECurveInterface::CurvePoint>* pVecPoints)
{
    QString s;
    char buffer[128];
    auto& vecPoints = pVecPoints == nullptr ? m_curvePoints : *pVecPoints;
    std::sort(vecPoints.begin(), vecPoints.end());
    std::for_each(vecPoints.begin(), vecPoints.end(), [&](decltype(vecPoints[0]) cp) {
        sprintf(buffer, "(%d, %d)", int(cp.x * 255.0f), int(cp.y * 255.0f));
        s += buffer;
    });
    m_curveEdit->setText(s);
}

void SingleCurveWidget::mousePressEvent(QMouseEvent* e)
{
    m_isHover = true;
}

void SingleCurveWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (m_isHover)
    {
        float fx = CGE::CGE_MID(float(e->x()) / width(), 0.0f, 1.0f);
        float fy = CGE::CGE_MID(1.0f - float(e->y()) / height(), 0.0f, 1.0f);

        m_curvePoints.push_back(CGE::CGECurveInterface::makeCurvePoint(fx, fy));
        validateText();
        drawCurveImage(m_curvePoints);
        m_curveAdjustWindow->updateFilter();
    }
    m_isHover = false;
}

void SingleCurveWidget::mouseMoveEvent(QMouseEvent* e)
{
    if (m_isHover)
    {
        float fx = CGE::CGE_MID(float(e->x()) / width(), 0.0f, 1.0f);
        float fy = CGE::CGE_MID(1.0f - float(e->y()) / height(), 0.0f, 1.0f);
        m_cachePoints = m_curvePoints;
        m_cachePoints.push_back(CGE::CGECurveInterface::makeCurvePoint(fx, fy));
        validateText(&m_cachePoints);
        drawCurveImage(m_cachePoints);
        m_curveAdjustWindow->updateFilter();
    }
}

//////////////////////////////////////////////////////////////////////////

CurveAdjustWindow::CurveAdjustWindow(MainWindow* mainWindow, QWidget* parent) :
    MenuDialogCommon(parent), m_mainWindow(mainWindow), m_filter(nullptr)
{
    m_ui.setupUi(this);
    setWindowFlags(Qt::Tool);
    m_mainWindow->enableMenu(false);
    setFixedSize(size());
    initWidgets();
    connect(m_ui.previewBtn, SIGNAL(clicked()), SLOT(preview()));
    connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(apply()));
}

void CurveAdjustWindow::initWidgets()
{
    m_rgbWidget = new SingleCurveWidget(this, m_ui.rgbEdit, SingleCurveWidget::Curve_RGB, m_ui.rgbWidget);
    m_rWidget = new SingleCurveWidget(this, m_ui.rEdit, SingleCurveWidget::Curve_R, m_ui.rWidget);
    m_gWidget = new SingleCurveWidget(this, m_ui.gEdit, SingleCurveWidget::Curve_G, m_ui.gWidget);
    m_bWidget = new SingleCurveWidget(this, m_ui.bEdit, SingleCurveWidget::Curve_B, m_ui.bWidget);
}

namespace CGE
{
extern bool g_isFastFilterImpossible;
}

bool CurveAdjustWindow::checkFilter()
{
    if (m_filter == nullptr)
    {
        m_filter =
            CGE::g_isFastFilterImpossible ? nullptr : static_cast<decltype(m_filter)>(CGE::createMoreCurveFilter());
        if (m_filter == nullptr) m_filter = static_cast<decltype(m_filter)>(CGE::createMoreCurveTexFilter());
        m_mainWindow->appendFilter(m_filter);
    }
    return !!m_filter;
}

void CurveAdjustWindow::updateFilter()
{
    m_mainWindow->useCanvasContext();
    if (!checkFilter()) return;

    auto& dataR = m_rWidget->getCurveData();
    auto& dataG = m_gWidget->getCurveData();
    auto& dataB = m_bWidget->getCurveData();
    m_filter->loadCurves(dataR.data(), dataR.size(), dataG.data(), dataG.size(), dataB.data(), dataB.size());

    auto& dataRGB = m_rgbWidget->getCurveData();
    m_filter->pushCurves(dataRGB.data(), dataRGB.size(), dataRGB.data(), dataRGB.size(), dataRGB.data(),
                         dataRGB.size());

    m_filter->flush();
    m_mainWindow->runFilters();
}

void CurveAdjustWindow::preview()
{
    if (!checkFilter()) return;

    m_filter->resetCurve();
    /////////////////////// R ////////////////////////////////////////
    {
        auto&& text = m_rWidget->getCurveText().toLocal8Bit();
        auto&& curvePoints = _genCurvePointWithData(text);
        if (curvePoints.size() >= 2)
        {
            m_rWidget->setCurvePoints(curvePoints);
            m_rWidget->drawCurveImage();
            m_rWidget->validateText();
            m_filter->setPointsR(curvePoints.data(), curvePoints.size());
        }
    }

    /////////////////////// G ////////////////////////////////////////
    {
        auto&& text = m_gWidget->getCurveText().toLocal8Bit();
        auto&& curvePoints = _genCurvePointWithData(text);
        if (curvePoints.size() >= 2)
        {
            m_gWidget->setCurvePoints(curvePoints);
            m_gWidget->drawCurveImage();
            m_gWidget->validateText();
            m_filter->setPointsG(curvePoints.data(), curvePoints.size());
        }
    }

    /////////////////////// B ////////////////////////////////////////
    {
        auto&& text = m_bWidget->getCurveText().toLocal8Bit();
        auto&& curvePoints = _genCurvePointWithData(text);
        if (curvePoints.size() >= 2)
        {
            m_bWidget->setCurvePoints(curvePoints);
            m_bWidget->drawCurveImage();
            m_bWidget->validateText();
            m_filter->setPointsB(curvePoints.data(), curvePoints.size());
        }
    }

    /////////////////////// RGB ////////////////////////////////////////
    {
        auto&& text = m_rgbWidget->getCurveText().toLocal8Bit();
        auto&& curvePoints = _genCurvePointWithData(text);
        if (curvePoints.size() >= 2)
        {
            m_rgbWidget->setCurvePoints(curvePoints);
            m_rgbWidget->drawCurveImage();
            m_rgbWidget->validateText();
            m_filter->pushPointsRGB(curvePoints.data(), curvePoints.size());
        }
    }

    m_filter->flush();
    m_mainWindow->runFilters();
}

void CurveAdjustWindow::apply()
{
    if (!checkFilter()) return;

    if (!m_filter) preview();

    auto& vecPointsRGB = m_rgbWidget->getCurvePoints();
    auto& vecPointsR = m_rWidget->getCurvePoints();
    auto& vecPointsG = m_gWidget->getCurvePoints();
    auto& vecPointsB = m_bWidget->getCurvePoints();

    FormatContent content;
    content.cge = "curve ";

    bool shouldApply = false;

    // Android and Flash outputs still need dedicated handling below.

    if (_isUsefulCurve(vecPointsR))
    {
        content.cge += QStringLiteral("R") + m_rWidget->getCurveText();
        shouldApply = true;
    }

    if (_isUsefulCurve(vecPointsG))
    {
        content.cge += QStringLiteral("G") + m_gWidget->getCurveText();
        shouldApply = true;
    }

    if (_isUsefulCurve(vecPointsB))
    {
        content.cge += QStringLiteral("B") + m_bWidget->getCurveText();
        shouldApply = true;
    }

    if (_isUsefulCurve(vecPointsRGB))
    {
        content.cge += QStringLiteral("RGB") + m_rgbWidget->getCurveText();
        shouldApply = true;
    }

    if (shouldApply)
    {
        m_filter->flush();
        m_mainWindow->runFilters();
        m_mainWindow->appendStep(m_filter, content);
        m_filter = nullptr;
        m_rgbWidget->resetWidgetCurve();
        m_rWidget->resetWidgetCurve();
        m_gWidget->resetWidgetCurve();
        m_bWidget->resetWidgetCurve();
        applyAndQuit();
    }
    else
    {
        QMessageBox::information(
            m_mainWindow, QStringLiteral("Invalid Parameters"),
            QStringLiteral(
                "Curve parameters are invalid or have no effect. Please ensure at least one curve is meaningful."));
        return;
    }
}

#define SSS(x) QString::fromLocal8Bit(x).toLocal8Bit()

bool CurveAdjustWindow::_isBrakeLeft(const char* separator)
{
    static QByteArray sBrakeLeft[] = {
        SSS("("), SSS("["), SSS("{"), SSS("（"), SSS("【"), SSS("｛"),
    };

    static int len = sizeof(sBrakeLeft) / sizeof(*sBrakeLeft);

    for (int i = 0; i != len; ++i)
    {
        if (strncmp(separator, sBrakeLeft[i], sBrakeLeft[i].size()) == 0)
        {
            return true;
        }
    }

    return false;
}

bool CurveAdjustWindow::_isBrakeRight(const char* separator)
{
    static QByteArray sBrakeRight[] = {
        SSS(")"), SSS("]"), SSS("}"), SSS("）"), SSS("】"), SSS("｝"),
    };

    static int len = sizeof(sBrakeRight) / sizeof(*sBrakeRight);

    for (int i = 0; i != len; ++i)
    {
        if (strncmp(separator, sBrakeRight[i], sBrakeRight[i].size()) == 0)
        {
            return true;
        }
    }

    return false;
}

std::vector<CGE::CGECurveInterface::CurvePoint> CurveAdjustWindow::_genCurvePointWithData(const char* dataText)
{
    std::vector<CGE::CGECurveInterface::CurvePoint> vec;
    while (*dataText && !_isBrakeLeft(dataText))
        ++dataText;
    int x, y;
    while (sscanf(dataText, "%*[^0-9]%d%*[^0-9]%d", &x, &y) == 2)
    {
        vec.push_back(CGE::CGECurveInterface::makeCurvePoint(x / 255.0f, y / 255.0f));
        while (*dataText && !_isBrakeRight(dataText))
            ++dataText;
        while (*dataText && !_isBrakeLeft(dataText))
            ++dataText;
    }
    std::sort(vec.begin(), vec.end());
    return vec;
}

bool CurveAdjustWindow::_isUsefulCurve(const std::vector<CGE::CGECurveInterface::CurvePoint>& cp) const
{
    if (cp.size() > 2) return true;
    if (cp.size() == 2)
    {
        auto& p0 = cp[0];
        auto& p1 = cp[1];

        return !(int(p0.x * 255.0f) == 0 && int(p0.y * 255.0f) == 0 && int(p1.x * 255.0f) == 255 &&
                 int(p1.y * 255.0f) == 255);
    }
    return false;
}
