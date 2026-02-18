/*
* canvasWidget.h
*
*  Created on: 2014-11-5
*      Author: Wang Yang
*        Mail: admin@wysaid.org
*/

#ifndef _CANVASWIDGET_H_
#define _CANVASWIDGET_H_

#include "cge.h"
#include <QOpenGLWidget>

// Special usage: Shared Texture will be replaced frequently, so do not use a reference or release it directly
class DisplaySprite : public CGE::Sprite2d
{
public:
	DisplaySprite(CGE::SharedTexture& tex) : CGE::Sprite2d(tex)
	{
		m_textureID = tex.texID();
	}

	// Select a new image, and switch
	void assignTexture(GLuint texID, int w, int h)
	{
		if(texID != 0)
			m_textureID = texID;
		m_texture.width = w;
		m_texture.height = h;
		m_program.bind();
		glUniform2f(m_halfTexLocation, m_texture.width / 2.0f, m_texture.height / 2.0f);
	}
	
	~DisplaySprite() {}

protected:
	void _drawFunc()
	{
		glBindBuffer(GL_ARRAY_BUFFER, CGE::CGEGlobalConfig::sVertexBufferCommon);
		glEnableVertexAttribArray(m_posAttribLocation);
		glVertexAttribPointer(m_posAttribLocation, 2, GL_FLOAT, false, 0, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
//		glUniform1i(m_textureLocation, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}


protected:
	GLuint m_textureID;
};

class CanvasWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	explicit CanvasWidget(QWidget* parent = nullptr);
	~CanvasWidget();

	bool openImage(const QString& filename);
	bool openImage(QImage& img);

	CGE::CGEImageHandler& getHandler() { return  m_handler;	}

	DisplaySprite* getDisplaySprite() { return m_sprite; }

	void imageZoom(float scaling);

	void setIntensity(float value);

public slots:

	void setToOriginImage();
	void setToCurrentImage();

	void restoreImage();
	void fitImage();
	
	void imageZoomIn();
	void imageZoomOut();

protected:
	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
	void keyReleaseEvent(QKeyEvent *);

	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);

	static CGEConstString paramTexSizeName;

private:
	DisplaySprite* m_sprite;
	int m_lastX, m_lastY;
	bool m_isMoving;

	GLint m_bgTextureLocation;
	GLuint m_posAttribLocation;
	CGE::ProgramObject* m_program;
	CGE::SharedTexture m_bgTexture;

	CGE::CGEImageHandler m_handler;	
	float m_intensity;
};

CGE::SharedTexture genSharedTextureWidthImageName(const char* filename, GLenum filter = GL_LINEAR);

#endif
