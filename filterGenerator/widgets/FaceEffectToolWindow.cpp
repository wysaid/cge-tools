/*
* faceEffectTool.cpp
*
*  Created on: 2017-4-12
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#include "faceEffectToolWindow.h"

using namespace CGE;

FaceSpriteWidget::FaceSpriteWidget(QWidget* parent, QOpenGLWidget* shareWidget) : QOpenGLWidget(parent), m_sprite(nullptr), m_pointDrawer(nullptr)
{
	connect(&m_timer, SIGNAL(timeout()), SLOT(updateProgress()));
	m_timer.setInterval(15);
	m_timer.stop();

	if(shareWidget != nullptr)
	{
		// Note: context sharing in Qt6 is set up at construction time via QOpenGLContext::setShareContext
	}
}

FaceSpriteWidget::~FaceSpriteWidget()
{
	delete m_sprite;
	CGE_LOG_ERROR("FaceSpriteWidget released...");
}

float FaceSpriteWidget::getProgress()
{
	if(m_sprite != nullptr && m_sprite->totalFrames() != 0)
		return m_sprite->currentFrame() / (float)m_sprite->totalFrames();
	return 0.0f;
}

void FaceSpriteWidget::updateProgress()
{
	if(m_sprite == nullptr)
		return;

	queueEvent([&](){

		if(m_sprite == nullptr)
			return;

		auto&& nowTime = QTime::currentTime();
		m_sprite->updateFrame(m_lastTime.msecsTo(nowTime));
		m_lastTime = nowTime;
	});

	update();
	emit progressUpdated();
}

void FaceSpriteWidget::setupWithJsonConfig(const QString& jsonConfig, const std::function<void (bool)>& callback)
{
	pausePlay();

	queueEvent([&, jsonConfig, callback](){

		m_jsonDoc = QJsonDocument::fromJson(jsonConfig.toLocal8Bit());

		bool isOK = false;

		cgeMakeBlockLimit([&](){

			if(callback != nullptr)
			{
				callback(isOK);
			}
		});

		if(!m_jsonDoc.isObject())
		{
			return;
		}

		QString&& currentDir = QDir::currentPath();

		if(currentDir.size() != 0 && currentDir[currentDir.size() - 1] != '/')
		{
			currentDir += "/";
		}

		currentDir += "sprite_asset/";

		QJsonObject jsonDoc = m_jsonDoc.object();
		qDebug() << jsonDoc;

		auto&& spriteConfig = jsonDoc["sprite_face"].toObject();

		if(spriteConfig.isEmpty())
		{
			spriteConfig = jsonDoc["sprite_face_multiple"].toObject();

			if(spriteConfig.isEmpty())
			{
				return;
			}
		}

		auto&& elements = spriteConfig["elements"].toArray();

		std::vector<Sprite2dInterChangeMultiple::FrameTexture> vecTexture;

		if(!elements.isEmpty())
		{
			Sprite2dInterChangeMultiple::FrameTexture tmpFrame;

			for(const auto& elem : elements)
			{
				QJsonObject value = elem.toObject();
				const QString& imageName = value["image"].toString();
				const QJsonArray& sz = value["size"].toArray();
				tmpFrame.col = sz[0].toInt();
				tmpFrame.row = sz[1].toInt();
				tmpFrame.count = value["count"].toInt();

				QString imageFilePath = currentDir + imageName;
				QImage&& img = QImage(imageFilePath).convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::ColorOnly);

				if(img.isNull())
				{
					return;
				}

				tmpFrame.textureID = cgeGenTextureWithBuffer(img.bits(), img.width(), img.height(), GL_RGBA, GL_UNSIGNED_BYTE);

				tmpFrame.width = img.width();
				tmpFrame.height = img.height();

				vecTexture.push_back(tmpFrame);
			}
		}

		if(!vecTexture.empty())
		{
			auto& firstFrameTex = vecTexture[0];

			m_spriteTextureSize.setWidth(firstFrameTex.width);
			m_spriteTextureSize.setHeight(firstFrameTex.height);
			m_spriteSize.setWidth(firstFrameTex.width / firstFrameTex.col);
			m_spriteSize.setHeight(firstFrameTex.height / firstFrameTex.row);

			if(m_sprite != nullptr)
				delete m_sprite;

			m_sprite = new Sprite2dInterChangeMultiple(m_spriteTextureSize.width(), m_spriteTextureSize.height());
			m_sprite->setFrameTextures(vecTexture);
			m_sprite->setHotspot(0.0f, 0.0f);
			m_sprite->setCanvasSize(width(), height());

			QJsonValue fpsValue = spriteConfig["fps"];
			double fps = 30.0;
			if(!fpsValue.isNull())
			{
				fps = fpsValue.toDouble();
			}

			m_sprite->setFrameDelayTime(1000.0 / fps);
			m_sprite->moveTo(width() / 2, height() / 2);

			m_sprite->jumpToFrame(0);

			m_vecPoints.resize(m_sprite->totalFrames());
			memset(m_vecPoints.data(), 0, m_vecPoints.size() * sizeof(m_vecPoints[0]));

			QJsonValue faceData = spriteConfig["faceData"];
			if(!faceData.isNull())
			{
				auto&& arrData = faceData.toArray();
				int len = MAX_POINTS * m_vecPoints.size() * 2;
				if(arrData.size() >= len)
				{
					for(int i = 0, n = 0; i < len; i += MAX_POINTS * 2, ++n)
					{
						for(int j = 0, k = 0; k != MAX_POINTS; j += 2,  ++k)
						{
							int index = i + j;
							auto& v = m_vecPoints[n].pnts[k];
							v[0] = arrData[index].toDouble();
							v[1] = arrData[index + 1].toDouble();

							CGE_LOG_INFO("%g, %g, ", v[0], v[1]);
						}

						CGE_LOG_INFO("\n");
					}
				}
			}
		}

		if(m_pointDrawer == nullptr)
		{
			m_pointDrawer = CGEPointDrawer::create();
			if(m_pointDrawer == nullptr)
				return;
			m_pointDrawer->setPointSize(2);
			m_pointDrawer->setColor(Vec4f(1.0, 0.0, 0.0, 1.0));
		}

		isOK = true;
	});
}

QString FaceSpriteWidget::genConfig()
{
	if(!m_jsonDoc.isObject())
		return "";

	QJsonArray jsonArr;

	auto& firstFace = m_vecPoints[0].pnts;
	float len = (firstFace[1] - firstFace[0]).length();

	for(auto& facePoints : m_vecPoints)
	{
#if 1 //FACE_SPRITE_MULTIPLE

		for(auto& pnts : facePoints.pnts)
		{
			jsonArr.append(pnts[0]);
			jsonArr.append(pnts[1]);
		}

#else

		auto&& centerPos = (facePoints.pnts[0] + facePoints.pnts[1]) * 0.5f;
		auto&& normDir = (facePoints.pnts[1] - facePoints.pnts[0]);
		normDir.normalize();
		auto&& halfDir = normDir * (len * 0.5f);
	
		auto&& pnt0 = centerPos - halfDir;
		auto&& pnt1 = centerPos + halfDir;

		jsonArr.append(pnt0[0]);
		jsonArr.append(pnt0[1]);

		jsonArr.append(pnt1[0]);
		jsonArr.append(pnt1[1]);

#endif
	}

	auto&& docObj = m_jsonDoc.object();

	const char* key = "sprite_face";

	auto&& spriteConfig = docObj.take(key);

	if(!spriteConfig.isObject())
	{
		key = "sprite_face_multiple";
		spriteConfig = docObj.take(key);
		if(!spriteConfig.isObject())
			return "";
	}

	auto&& spriteObj = spriteConfig.toObject();
	spriteObj.insert("faceData", jsonArr);

	docObj.insert(key, spriteObj);

	m_jsonDoc.setObject(docObj);

	QString s = m_jsonDoc.toJson(QJsonDocument::Indented);

	return s;
}

void FaceSpriteWidget::queueEvent(std::function<void ()>&& func)
{
	m_dispatchFunctions.push_back(func);

    update();
}

void FaceSpriteWidget::startPlay()
{
	if(m_sprite != nullptr)
	{
		m_lastTime = QTime::currentTime();
		m_timer.start();
	}
}

void FaceSpriteWidget::pausePlay()
{
	if(m_sprite != nullptr)
	{
		m_timer.stop();
	}
}

void FaceSpriteWidget::jumpFrame(float progress)
{
	if(m_sprite == nullptr)
		return;

	queueEvent([=](){
		m_sprite->jumpToFrame(progress * (m_sprite->totalFrames() - 1));
	});

	update();
}

void FaceSpriteWidget::paintGL()
{
	while(!m_dispatchFunctions.empty())
	{
		auto&& func = m_dispatchFunctions.front();
		func();
		m_dispatchFunctions.pop_front();		
	}

#ifndef Q_OS_IOS
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());

	if(m_sprite != nullptr)
	{
		m_sprite->render();

		if(m_pointDrawer != nullptr && m_vecPoints.size() == m_sprite->totalFrames())
		{
			auto& facePnts = m_vecPoints[m_sprite->currentFrame()];
			m_pointDrawer->draw(facePnts.pnts, MAX_POINTS);
		}
	}

}

void FaceSpriteWidget::_calcDrawPoints()
{
	if(m_vecPoints.size() == m_sprite->totalFrames())
	{
		m_vecDrawPoints = m_vecPoints;
		
		Vec2f sz(width(), height());
		Vec2f spriteSz(m_spriteSize.width(), m_spriteSize.height());
		Vec2f spriteSzRev(1.0f / spriteSz[0], 1.0f / spriteSz[1]);

		for(int i = 0; i != m_vecDrawPoints.size(); ++i)
		{
			auto& facePoints = m_vecDrawPoints[i];
			for(auto& pnts : facePoints.pnts)
			{
				pnts *= spriteSzRev * 2.0f;
				pnts -= 1.0f;
			}
			m_pointDrawer->draw(facePoints.pnts, MAX_POINTS);
		}
		
	}
}

void FaceSpriteWidget::resizeGL(int w, int h)
{
	if(m_sprite != nullptr)
	{
		// Do not use actual pixel size; this keeps sprite size consistent across Retina displays
		m_sprite->setCanvasSize(width(), height());
		m_sprite->moveTo(width() / 2, height() / 2);
	}
}

bool FaceSpriteWidget::isPlaying()
{
	return m_timer.isActive();
}

int FaceSpriteWidget::getCurrentFrame()
{
	return m_sprite == nullptr ? 0 : m_sprite->currentFrame();
}

//////////////////////////////////////////////////////////////////////////

// FaceEffectToolMarkWidgetCanvas::FaceEffectToolMarkWidgetCanvas(QWidget* parent, FaceSpriteWidget* shareWidget) : QGLWidget(parent, shareWidget), m_spriteWidget(shareWidget)
// {
// 	
// };
// 
// void FaceEffectToolMarkWidgetCanvas::paintGL()
// {
// 	auto* sprite = m_spriteWidget->sprite();
// 
// 	if(sprite != nullptr)
// 	{
// 		sprite->render();
// 	}
// }
// 
// void FaceEffectToolMarkWidgetCanvas::resizeGL(int w, int h)
// {
// 	
// }

//////////////////////////////////////////////////////////////////////////

FaceEffectToolMarkWidget::FaceEffectToolMarkWidget(FaceEffectToolWindow* parent, const std::function<void (std::vector<FaceSpriteWidget::FacePoints>*)>& func) : MenuDialogCommon(parent), m_callback(func), m_currentTarget(0)
{
	setWindowTitle("Please mark the required keypoints in the image");
	setFixedSize(parent->spriteWidget()->spriteSize());

// 	m_canvas = new FaceEffectToolMarkWidgetCanvas(this, parent->spriteWidget());
// 	m_canvas->setGeometry(0, 0, width(), height());

	m_parent = parent;

	assert(m_parent != nullptr);

	parent->spriteWidget()->makeCurrent();

	m_framebuffer = new FrameBuffer;
	m_cacheTexture = cgeGenTextureWithBuffer(nullptr, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE);
	m_framebuffer->bindTexture2D(m_cacheTexture);

	auto* sprite = parent->spriteWidget()->sprite();
	if(sprite != nullptr)
	{
		sprite->setCanvasFlip(false, false);
		sprite->setCanvasSize(width(), height());
		sprite->moveTo(width() / 2, height() / 2);
	}

	parent->spriteWidget()->doneCurrent();

	flushCache();

	m_vecPoints.resize(sprite->totalFrames());

	memset(m_vecPoints.data(), 0, sizeof(m_vecPoints[0]) * m_vecPoints.size());
	grabKeyboard();

	m_vecPoints = parent->spriteWidget()->getPoints();
}

FaceEffectToolMarkWidget::~FaceEffectToolMarkWidget()
{
	if(m_callback != nullptr)
	{
		m_callback(&m_vecPoints);
	}

	m_parent->spriteWidget()->makeCurrent();
	delete m_framebuffer;
	glDeleteTextures(1, &m_cacheTexture);
};

// void FaceEffectToolMarkWidget::resizeEvent(QResizeEvent *e)
// {
// 	
// }

void FaceEffectToolMarkWidget::keyReleaseEvent(QKeyEvent *e)
{
	auto* w = m_parent->spriteWidget();

	w->makeCurrent();
	auto* sprite = w->sprite();

	switch (e->key())
	{
	case Qt::Key_Right:
        if(sprite->currentFrame() != sprite->totalFrames() - 1)
            sprite->nextFrame(1);
//		m_currentTarget = 0;
//        m_currentTarget += 3;
		break;
	case Qt::Key_Left:
        if(sprite->currentFrame() != 0)
            sprite->jumpToFrame(sprite->currentFrame() - 1);
//		m_currentTarget = 0;
//        m_currentTarget -= 3;
		break;
    case Qt::Key_0: case Qt::Key_1: case Qt::Key_2: case Qt::Key_3: case Qt::Key_4: case Qt::Key_5: case Qt::Key_6: case Qt::Key_7: case Qt::Key_8: case Qt::Key_9:
        m_currentTarget = e->key() - Qt::Key_0;
		break;
    case Qt::Key_Up:
        ++m_currentTarget;
        break;
    case Qt::Key_Down:
        --m_currentTarget;
        break;
    case Qt::Key_W:
        m_vecPoints[m_parent->spriteWidget()->getCurrentFrame()].pnts[m_currentTarget][1] -= 1;
        break;
    case Qt::Key_A:
        m_vecPoints[m_parent->spriteWidget()->getCurrentFrame()].pnts[m_currentTarget][0] -= 1;
        break;
    case Qt::Key_S:
        m_vecPoints[m_parent->spriteWidget()->getCurrentFrame()].pnts[m_currentTarget][1] += 1;
        break;
    case Qt::Key_D:
        m_vecPoints[m_parent->spriteWidget()->getCurrentFrame()].pnts[m_currentTarget][0] += 1;
        break;
	default:
		break;
    }

    if(m_currentTarget < 0)
        m_currentTarget = 0;
    else if(m_currentTarget > FaceSpriteWidget::MAX_POINTS - 1)
        m_currentTarget = FaceSpriteWidget::MAX_POINTS - 1;

	w->doneCurrent();

	if(e->key() == Qt::Key_Left || e->key() == Qt::Key_Right)
	{
		flushCache();
	}

	update();
}

void FaceEffectToolMarkWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
		return;

	auto currentFrame = m_parent->spriteWidget()->getCurrentFrame();

	if(e->button() == Qt::MiddleButton && currentFrame != 0)
	{
		m_vecPoints[currentFrame] = m_vecPoints[currentFrame - 1];
	}
	else
	{
		m_vecPoints[currentFrame].pnts[m_currentTarget] = Vec2f(e->x(), e->y());
//		++m_currentTarget;
//		m_currentTarget %= FaceSpriteWidget::MAX_POINTS;
	}
	
	update();
}

void FaceEffectToolMarkWidget::flushCache()
{
	auto* widget = m_parent->spriteWidget();

	widget->makeCurrent();

	m_framebuffer->bind();

	auto&& sz = widget->spriteSize();
	glViewport(0, 0, sz.width(), sz.height());

	m_parent->spriteWidget()->sprite()->render();

	if(m_cacheImage.isNull() || m_cacheImage.width() != sz.width() || m_cacheImage.height() != sz.height())
	{
		m_cacheImage = QImage(sz.width(), sz.height(), QImage::Format_RGBA8888_Premultiplied);
	}

	glReadPixels(0, 0, sz.width(), sz.height(), GL_RGBA, GL_UNSIGNED_BYTE, m_cacheImage.bits());

	widget->doneCurrent();
}

void FaceEffectToolMarkWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	painter.setPen(Qt::red);

	painter.drawImage(0, 0, m_cacheImage);

	auto* sprite = m_parent->spriteWidget()->sprite();

	painter.drawText(0, 30, QString("Current frame: %1, Selected keypoint: %2").arg(sprite->currentFrame()).arg(m_currentTarget));

	auto& pnts = m_vecPoints[sprite->currentFrame()];

	for(int i = 0; i != FaceSpriteWidget::MAX_POINTS; ++i)
	{
		auto& p = pnts.pnts[i];
		painter.setPen(Qt::blue);
		painter.drawEllipse(QPoint(p[0], p[1]), 2, 2);
		painter.setPen(Qt::black);
		painter.drawText(p[0], p[1], QStringLiteral("%1").arg(i));
	}
}

//////////////////////////////////////////////////////////////////////////

FaceEffectToolWindow::FaceEffectToolWindow(const std::function<void ()>& func, MainWindow* mainwindow) : QWidget(nullptr), m_destructFunc(func)
{
	m_ui.setupUi(this);
	m_mainWindow = mainwindow;
	setAttribute(Qt::WA_DeleteOnClose);

	m_ui.progressSlider->setRange(0, 100);

	m_spriteWidget = new FaceSpriteWidget(this, mainwindow->getCanvasWidget());
	m_spriteWidget->setGeometry(m_ui.contentWidget->geometry());
	m_ui.contentWidget->hide();
	m_spriteWidget->setStyleSheet("background-color:#f00");

	m_spriteWidget->connect(m_ui.playBtn, SIGNAL(clicked()), SLOT(startPlay()));
	m_spriteWidget->connect(m_ui.pauseBtn, SIGNAL(clicked()), SLOT(pausePlay()));

	connect(m_ui.applyBtn, SIGNAL(clicked()), SLOT(applyConfig()));
	connect(m_ui.saveBtn, SIGNAL(clicked()), SLOT(saveConfig()));
	connect(m_ui.jumpBtn, SIGNAL(clicked()), SLOT(jumpToFixedFrame()));
	connect(m_ui.progressSlider, SIGNAL(valueChanged(int)), SLOT(jumpToFrame(int)));
	connect(m_spriteWidget, SIGNAL(progressUpdated()), SLOT(updateProgress()));
	connect(m_ui.dirBtn, SIGNAL(clicked()), SLOT(setConfigDir()));

	connect(m_ui.textLeftBtn, SIGNAL(clicked()), SLOT(textLeftBtnClicked()));
	connect(m_ui.textRightBtn, SIGNAL(clicked()), SLOT(textRightBtnClicked()));

	connect(m_ui.markFrameBtn, SIGNAL(clicked()), SLOT(markFrame()));
}

FaceEffectToolWindow::~FaceEffectToolWindow()
{
	windowClose();
}

void FaceEffectToolWindow::windowClose()
{
	if(m_destructFunc)
	{
		m_destructFunc();
		m_destructFunc = nullptr;
	}
}

void FaceEffectToolWindow::_fixLayout()
{
	auto sz = size();

	{
		auto rt = m_spriteWidget->geometry();
		rt.setWidth(sz.width() - rt.left() - 10);
		rt.setHeight(sz.height() - rt.top() - 10);
		m_spriteWidget->setGeometry(rt);
			m_spriteWidget->update();
	}

	{
		auto rt = m_ui.configEdit->geometry();
		rt.setWidth(sz.width() - m_spriteWidget->width() - 20 - rt.left());
		rt.setHeight(sz.height() - m_ui.configEdit->geometry().top() - 10);
		m_ui.configEdit->setGeometry(rt);
	}
	
}

void FaceEffectToolWindow::resizeEvent(QResizeEvent *)
{
	_fixLayout();
}

void FaceEffectToolWindow::textLeftBtnClicked()
{
	_moveUI(10);
}

void FaceEffectToolWindow::textRightBtnClicked()
{
	_moveUI(-10);
}

void FaceEffectToolWindow::_moveUI(int n)
{
	auto p = m_spriteWidget->pos();
	p.setX(p.x() + n);
	m_spriteWidget->move(p);
	_fixLayout();
}

void FaceEffectToolWindow::closeEvent(QCloseEvent *e)
{
	windowClose();
	e->accept();
}

void FaceEffectToolWindow::applyConfig()
{
	m_spriteWidget->setupWithJsonConfig(m_ui.configEdit->toPlainText(), [&](bool suc){
		if(!suc)
		{
			QMessageBox::warning(this, "waring!", "apply config failed!");
		}
	});
}

void FaceEffectToolWindow::saveConfig()
{
	auto&& s = m_spriteWidget->genConfig();
	m_ui.configEdit->setText(s);
}

void FaceEffectToolWindow::jumpToFixedFrame()
{
	int frame = m_ui.frameEdit->text().toInt();
	m_spriteWidget->jumpFrame(frame);
}

void FaceEffectToolWindow::jumpToFrame(int frameIndex)
{
	if(!m_spriteWidget->isPlaying())
	{
		m_spriteWidget->jumpFrame(frameIndex / 100.0f);
	}

	m_ui.frameEdit->setText(QStringLiteral("%1").arg(m_spriteWidget->getCurrentFrame()));
}

void FaceEffectToolWindow::updateProgress()
{
	float value = m_spriteWidget->getProgress();
	m_ui.progressSlider->setValue(value * m_ui.progressSlider->maximum());
}

void FaceEffectToolWindow::setConfigDir()
{
	QString&& dir = QFileDialog::getExistingDirectory(this, "Choose a folder", QDir::currentPath());

	if(!dir.isEmpty())
	{
		QDir::setCurrent(dir);
		CGE_LOG_INFO("Current dir: %s\n", (const char*)dir.toLocal8Bit());

		QString filename = dir + "/params.txt";
		QFile file(filename);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		if(file.isReadable())
		{
            m_ui.controlWidget->setEnabled(false);
			auto filesize = file.size();
			std::vector<char> vecData(filesize + 1);
			file.read(vecData.data(), filesize);
			vecData[filesize] = '\0';
			m_spriteWidget->setupWithJsonConfig(vecData.data(), [&, vecData](bool suc){
				if(suc)
				{
					m_ui.configEdit->setText(vecData.data());
				}
				else
				{
					QMessageBox::critical(this, "failed!", "Open file failed!");
				}

                m_ui.controlWidget->setEnabled(true);
                update();
			});
		}
        else
        {
            QMessageBox::critical(this, "failed!", "Config file does not exist!");
        }
	}
	else
	{
		CGE_LOG_INFO("cancel: choose dir.\n");
	}

}

void FaceEffectToolWindow::markFrame()
{
	if(m_spriteWidget->sprite()  == nullptr)
		return;

	m_spriteWidget->pausePlay();
	m_spriteWidget->hide();

	FaceEffectToolMarkWidget* widget = new FaceEffectToolMarkWidget(this, [&](std::vector<FaceSpriteWidget::FacePoints>* pnts){

		auto* widget = m_spriteWidget;

		if(pnts != nullptr)
		{
			widget->setPoints(*pnts);
		}

		m_spriteWidget->queueEvent([widget](){
			auto* sprite = widget->sprite();
			if(sprite != nullptr)
			{
				sprite->setCanvasSize(widget->width(), widget->height());
				sprite->moveTo(widget->width() / 2, widget->height() / 2);
				sprite->setCanvasFlip(false, true);
			}
		});
		m_spriteWidget->show();
	});

	widget->exec();
}
