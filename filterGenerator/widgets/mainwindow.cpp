/*
 * mainwindow.cpp
 *
 *  Created on: 2014-11-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#include "mainwindow.h"
#include "cgeMultipleEffectsDataset.h"

#define MARGIN_TOP 10

FilterFormats::Format FilterFormats::brightness("adjust brightness %1", "", "");

FilterFormats::Format FilterFormats::contrast("adjust contrast %1", "", "");

FilterFormats::Format FilterFormats::saturation("adjust saturation %1", "", "");

FilterFormats::Format FilterFormats::exposure("adjust exposure %1", "", "");

FilterFormats::Format FilterFormats::hue("adjust hue %1", "", "");

FilterFormats::Format FilterFormats::shadowHighlight("adjust shadowhighlight %1 %2", "", "");

FilterFormats::Format FilterFormats::blurSharpen("adjust %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::vignette("vignette %1 %2", "", "");

FilterFormats::Format FilterFormats::hsl("adjust hsl %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::hsv("adjust hsv %1 %2 %3 %4 %5 %6", "", "");

FilterFormats::Format FilterFormats::monochrome("adjust monochrome %1 %2 %3 %4 %5 %6", "", "");

FilterFormats::Format FilterFormats::whiteBalance("adjust whitebalance %1 %2", "", "");

FilterFormats::Format FilterFormats::colorBalance("adjust colorbalance %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::colorLevel("adjust level %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::texBlend("blend %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::texBlendKeepRatio("krblend %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::tileBlend("tileblend %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::pixelBlend("pixblend %1 %2 %3 %4 %5 %6", "", "");

FilterFormats::Format FilterFormats::vignetteBlend("vigblend %1 %2 %3 %4 %5 %6 %7 %8 0.5 0.5 3", "", "");

FilterFormats::Format FilterFormats::vignetteBlendWithNoAlpha("vigblend %1 %2 %3 %4 %5 %6 %7 %8 0.5 0.5 2", "", "");

FilterFormats::Format FilterFormats::selfBlend("selfblend %1 %2", "", "");

FilterFormats::Format FilterFormats::crossHatch("style crosshatch %1 %2", "", "");

FilterFormats::Format FilterFormats::edge("style edge %1 %2", "", "");

FilterFormats::Format FilterFormats::emboss("style emboss %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::halftone("style halftone %1", "", "");

FilterFormats::Format FilterFormats::haze("style haze %1 %2 %3 %4 %5", "", "");

FilterFormats::Format FilterFormats::polkadot("style polkadot %1", "", "");

FilterFormats::Format FilterFormats::sketch("style sketch %1", "", "");

FilterFormats::Format FilterFormats::max("style max", "", "");

FilterFormats::Format FilterFormats::min("style min", "", "");

FilterFormats::Format FilterFormats::mid("style mid", "", "");

FilterFormats::Format FilterFormats::bilateralBlur("beautify bilateral %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::lerpBlur("blur lerp %1 %2", "", "");

FilterFormats::Format FilterFormats::wave("dynamic wave %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::lookup("adjust lut %1", "", "");

FilterFormats::Format FilterFormats::colorMapping("style cm %1 %2 %3 %4 %5", "", "");

FilterFormats::Format FilterFormats::colorMulVec("style colormul vec %1 %2 %3", "", "");

FilterFormats::Format FilterFormats::colorMulMat("style colormul mat %1 %2 %3 %4 %5 %6 %7 %8 %9", "", "");

//////////////////////////////////////////////////////////////////////////

void MenuDialogCommon::applyAndQuit()
{
    CGE_LOG_INFO("Apply Success!\n");
    QMessageBox::information(this, QStringLiteral("Effect Added"), QStringLiteral("The effect was added successfully."),
                             QMessageBox::Ok);
    deleteLater();
}

//////////////////////////////////////////////////////////////////////////

ExportWindow::ExportWindow(QWidget* parent) : MenuDialogCommon(parent)
{
    m_ui.setupUi(this);
    connect(m_ui.exportBtn, SIGNAL(clicked()), SLOT(pathChooseBtnClicked()));
    connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(applyBtnClicked()));
    connect(m_ui.cancelBtn, SIGNAL(clicked()), SLOT(cancelBtnClicked()));
}

void ExportWindow::pathChooseBtnClicked()
{
    QString&& folderPath =
        QFileDialog::getExistingDirectory(this, QStringLiteral("Choose A Folder!"), QDir::currentPath());
    if (folderPath.isEmpty())
    {
        CGE_LOG_INFO("Cancel 'exportInternalPackage'");
        return;
    }
    m_ui.pathEdit->setText(folderPath);
}

void ExportWindow::_saveAsCGE()
{
    QString ver(cgeGetVersion());

    ver.replace('.', '_');

    QString path = m_ui.pathEdit->text();

    for (int i = 0; i != CGEEffectsDataSet::g_cgeEffectNum; ++i)
    {
        QString&& data = QString::fromLocal8Bit(CGEEffectsDataSet::cgeGetEffectStringByID(i));
        if (data.indexOf("unavailable") >= 0) continue;
        QString&& fileName = QString(path + "/effect.%1.v%2.algorithm").arg(i).arg(ver);
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(data.toLocal8Bit());
            file.close();
        }
        else
        {
            CGE_LOG_INFO("wirte file %s failed\n", (const char*)fileName.toLocal8Bit());
        }
    }
}

void ExportWindow::_saveAsAndroid()
{
    CGE_LOG_ERROR("\nNot supported yet\n");
}

void ExportWindow::_saveAsFlash()
{
    CGE_LOG_ERROR("\nNot supported yet\n");
}

void ExportWindow::applyBtnClicked()
{
    //	if(m_ui.cgeBtn->isChecked())
    //		_saveAsCGE();
    //	else if(m_ui.androidBtn->isChecked())
    //		_saveAsAndroid();
    //	else
    //		_saveAsFlash();

    QMessageBox::information(this, "Failed", QString::fromLocal8Bit("Not provided by this version!"));
    deleteLater();
}

void ExportWindow::cancelBtnClicked()
{
    deleteLater();
}

//////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_stopRunFilters(false), m_shouldSave(false)
{
    m_ui.setupUi(this);
    _initWidgets();

    setFocus();

    connect(m_ui.openImageBtn, SIGNAL(clicked()), SLOT(openImage()));
    connect(m_ui.saveImageBtn, SIGNAL(clicked()), SLOT(saveResults()));
    connect(m_ui.previewConfigBtn, SIGNAL(clicked()), SLOT(previewResults()));
    //	connect(m_ui.exportBtn, SIGNAL(clicked()), SLOT(exportInternalPackage()));
    connect(m_ui.useLUTBtn, SIGNAL(clicked()), SLOT(useLUT()));

    //////////////////////////////////////////////////////////////////////////

    connect(m_ui.resizeBtn, SIGNAL(clicked()), m_canvasWidget, SLOT(restoreImage()));
    connect(m_ui.compBtn, SIGNAL(pressed()), m_canvasWidget, SLOT(setToOriginImage()));
    connect(m_ui.compBtn, SIGNAL(released()), m_canvasWidget, SLOT(setToCurrentImage()));

    connect(m_ui.fitBtn, SIGNAL(clicked()), m_canvasWidget, SLOT(fitImage()));
    connect(m_ui.zoomInBtn, SIGNAL(clicked()), m_canvasWidget, SLOT(imageZoomIn()));
    connect(m_ui.zoomOutBtn, SIGNAL(clicked()), m_canvasWidget, SLOT(imageZoomOut()));
}

MainWindow::~MainWindow()
{
    delete m_canvasWidget;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    const int borderSize = 10;
    auto w = width();
    auto h = height();
    m_ui.centralWidget->setGeometry(0, 0, w, h);

    {
        auto g = m_ui.adjustWidget->geometry();
        m_ui.adjustWidget->setGeometry(g.x(), g.y(), g.width(), h - g.y() - borderSize);
        m_menuWidget->setGeometry(m_ui.adjustWidget->geometry());
    }
    {
        auto g = m_ui.sceneWidget->geometry();
        auto rightBorder = m_ui.controlWidget->width() + borderSize;
        m_ui.sceneWidget->setGeometry(g.x(), g.y(), w - rightBorder - g.x(), h - g.y() - borderSize);
        m_canvasWidget->setGeometry(QRect({0, 0}, m_ui.sceneWidget->size()));
    }
    {
        auto g = m_ui.controlWidget->geometry();
        m_ui.controlWidget->setGeometry(w - g.width() - borderSize / 2, g.y(), g.width(), h - g.y() - borderSize);
        g = m_ui.outputScrollWidget->geometry();
        auto g2 = m_ui.controlWidget->geometry();
        m_ui.outputScrollWidget->setGeometry(g.x(), g.y(), g.width(), g2.height() - g.y());
    }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (m_shouldSave && !m_resultSteps.empty())
    {
        if (QMessageBox::question(this, QStringLiteral("Unsaved Work"),
                                  QStringLiteral("You have unsaved changes. Exit anyway?"), QMessageBox::Yes,
                                  QMessageBox::No) == QMessageBox::No)
        {
            e->ignore();
            return;
        }
    }

    m_resultSteps.clear();
    m_canvasWidget->makeCurrent();
    CGE::cgeCleanup();
}

void MainWindow::_initWidgets()
{
    m_menuWidget = new MenuWidget(this, m_ui.centralWidget);
    m_menuWidget->setGeometry(m_ui.adjustWidget->geometry());
    m_ui.adjustWidget->hide();
    m_menuWidget->show();

    m_canvasWidget = new CanvasWidget(m_ui.sceneWidget);
    m_canvasWidget->setGeometry(QRect({0, 0}, m_ui.sceneWidget->size()));
    m_canvasWidget->show();
}

void MainWindow::openImage()
{
    QString&& filename = QFileDialog::getOpenFileName(this, QStringLiteral("Select an image file!"), "",
                                                      QStringLiteral("Images (*.jpg *.png *.gif *.xpm)"));

    if (filename.size() == 0)
    {
        return;
    }

    if (!m_canvasWidget->openImage(filename))
    {
        QMessageBox::warning(this, QStringLiteral("Unexpected Error"),
                             QStringLiteral("Unable to write config file: %1").arg(filename));
    }
}

void MainWindow::saveResults()
{
    if (m_resultSteps.empty())
    {
        QMessageBox::warning(this, QStringLiteral("Nothing to Save"), QStringLiteral("There is nothing to save."));
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, QStringLiteral("Save The Result"), QDir::currentPath(),
                                                    "Config (*.cge);;All files (*)");
    if (filename.size() == 0)
    {
        return;
    }
    _saveImage(filename);
    _saveConfig(filename);
    m_shouldSave = false;
}

void MainWindow::previewResults()
{
    CGE_LOG_INFO("previewResults\n");

    QString configCGE;
    std::for_each(m_resultSteps.begin(), m_resultSteps.end(), [&](decltype(m_resultSteps[0])& step) {
        auto& result = step->getResult();
        configCGE += QStringLiteral("@") + result.formatContent.cge + " ";
    });

    if (configCGE.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("No Effects"),
                                 QStringLiteral("No effects have been applied yet."));
        return;
    }

    QPlainTextEdit* w = new QPlainTextEdit(this);
    w->setWindowFlags(Qt::Tool);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setFixedSize(400, 300);
    w->setWindowOpacity(0.85);
    w->setReadOnly(true);
    w->setWindowModality(Qt::ApplicationModal);
    w->setPlainText(configCGE);
    w->show();
}

void MainWindow::useLUT()
{
    if (!m_canvasWidget->openImage(":CGE/resource/lookup.png"))
    {
        CGE_LOG_ERROR("Failed to load LUT image!\n");
    }
}

void MainWindow::_saveString(const QString& filename, const QString& data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, QStringLiteral("Unexpected Error"),
                             QStringLiteral("Unable to write config file: %1").arg(filename));
        return;
    }

    file.write(data.toUtf8());
}

void MainWindow::_saveConfig(const QString& filename)
{
    QString configCGE, configAndroid, configFlash;
    std::for_each(m_resultSteps.begin(), m_resultSteps.end(), [&](decltype(m_resultSteps[0])& step) {
        auto& result = step->getResult();
        configCGE += QStringLiteral("@") + result.formatContent.cge + " ";
        configAndroid += QStringLiteral("android") + result.formatContent.android + " ";
        configFlash += QStringLiteral("flash") + result.formatContent.flash + " ";
    });

    _saveString(filename + "_cge.txt", configCGE);
    //	_saveString(filename + "_android.xml", configAndroid);
    //	_saveString(filename + "_flash.xml", configFlash);
}

void MainWindow::_saveImage(const QString& filename)
{
    auto&& name = filename + QStringLiteral(".png");
    if (QFile::exists(name))
    {
        if (QMessageBox::question(this, QStringLiteral("File Conflict"),
                                  QStringLiteral("%1 already exists. Overwrite it?").arg(name), QMessageBox::Yes,
                                  QMessageBox::No) == QMessageBox::No)
            return;
    }
    auto& handler = m_canvasWidget->getHandler();
    auto sz = handler.getOutputFBOSize();

    QImage img(sz.width, sz.height, QImage::Format_RGBA8888);
    handler.getOutputBufferData(img.bits(), CGE_FORMAT_RGBA_INT8);
    img.save(name);
}

void MainWindow::appendStep(CGE::CGEImageFilterInterfaceAbstract* filter, const FormatContent& content)
{
    if (filter == nullptr)
    {
        QMessageBox::information(this, QStringLiteral("Invalid Filter"),
                                 QStringLiteral("Apply failed due to an unknown error. Please contact the author."));
        return;
    }

    auto w = new ResultWidget(this, m_ui.outputWidget);
    w->setHTML(content.cge);
    FilterResultFormat result(content, filter);
    w->setResult(result);
    w->setGeometry(3, m_resultSteps.size() * (w->height() + MARGIN_TOP), w->width(), w->height());
    w->show();
    m_resultSteps.push_back(w);
    m_ui.outputWidget->setFixedHeight(
        CGE::CGE_MAX((MARGIN_TOP + w->height()) * (int)m_resultSteps.size(), m_ui.outputScrollWidget->height() - 20));
    m_ui.outputScrollWidget->verticalScrollBar()->setValue(m_ui.outputWidget->height());

    m_shouldSave = true;
}

bool MainWindow::removeStep(ResultWidget* step)
{
    auto widgetIter = std::find(m_resultSteps.begin(), m_resultSteps.end(), step);
    if (widgetIter != m_resultSteps.end())
    {
        m_resultSteps.erase(widgetIter);
        refreshOutputWidget();
        removeFilter(step->getResult().filter);
        step->deleteLater();
        m_shouldSave = true;
        CGE_LOG_INFO("A result step is removed!\n");
        return true;
    }
    return false;
}

void MainWindow::sortSteps()
{
    std::sort(m_resultSteps.begin(), m_resultSteps.end(), ResultWidget::sortComp);

    int h = 0;
    std::for_each(m_resultSteps.begin(), m_resultSteps.end(), [&](ResultWidget* w) {
        if (!w->isHover())
        {
            w->setGeometry(w->x(), h, w->width(), w->height());
        }
        h += w->height() + MARGIN_TOP;
    });
}

void MainWindow::enableMenu(bool useMenu)
{
    if (this->isVisible())
    {
        m_menuWidget->setEnabled(useMenu);
        m_ui.outputWidget->setEnabled(useMenu);
    }
}

void MainWindow::refreshOutputWidget()
{
    int height = 0;
    for (auto widget : m_resultSteps)
    {
        widget->setGeometry(3, height, widget->width(), widget->height());
        height += (widget->height() + MARGIN_TOP);
        widget->show();
    }
    m_ui.outputWidget->setFixedHeight(CGE::CGE_MAX(height, m_ui.outputScrollWidget->height() - 20));
    m_ui.outputScrollWidget->verticalScrollBar()->setValue(m_ui.outputWidget->height());
}

void MainWindow::appendFilter(CGE::CGEImageFilterInterfaceAbstract* filter)
{
    auto& handler = m_canvasWidget->getHandler();
    handler.addImageFilter(filter);
}

void MainWindow::useCanvasContext()
{
    m_canvasWidget->makeCurrent();
}

void MainWindow::runFilters()
{
    if (m_stopRunFilters) return;
    m_canvasWidget->makeCurrent();
    auto& handler = m_canvasWidget->getHandler();
    handler.revertToKeptResult(false);
    handler.processingFilters();

    auto sp = m_canvasWidget->getDisplaySprite();
    auto sz = handler.getOutputFBOSize();
    sp->assignTexture(handler.getTargetTextureID(), sz.width, sz.height);

    m_canvasWidget->update();
}

void MainWindow::removeFilter(CGE::CGEImageFilterInterfaceAbstract* filter, bool bDel)
{
    if (!isVisible()) return;
    m_canvasWidget->makeCurrent();
    auto& handler = m_canvasWidget->getHandler();
    if (!handler.deleteFilterByAddr(filter, bDel))
    {
        CGE_LOG_ERROR("removeFilter failed!\n");
    }
    else
    {
        CGE_LOG_INFO("A filter is removed!\n");
        runFilters();
    }
}

void MainWindow::sortFilters()
{
    m_canvasWidget->makeCurrent();
    auto& handler = m_canvasWidget->getHandler();
    auto& filters = handler.peekFilters();

    if (filters.size() == m_resultSteps.size())
    {
        bool flag = true;
        for (decltype(filters.size()) i = 0; i != filters.size(); ++i)
        {
            if (filters[i] != m_resultSteps[i]->getResult().filter)
            {
                flag = false;
                break;
            }
        }
        if (flag) return;
    }

    handler.clearImageFilters(false);

    std::for_each(m_resultSteps.begin(), m_resultSteps.end(), [&](ResultWidget* w) {
        auto& result = w->getResult();
        handler.addImageFilter(result.filter);
    });

    runFilters();
    m_shouldSave = true;
    return;
}
