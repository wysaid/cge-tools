/*
 * cgeTextureUtils.h
 *
 *  Created on: 2015-7-29
 *      Author: Wang Yang
 */

#ifndef _CGETEXTUREUTILS_H_
#define _CGETEXTUREUTILS_H_

#include "cgeGLFunctions.h"

namespace CGE
{

class TextureDrawer
{
protected:
    TextureDrawer() : m_vertBuffer(0) {}
    virtual bool init();

public:
    virtual ~TextureDrawer();

    CGE_COMMON_CREATE_FUNC(TextureDrawer, init);

    // Draw texture to the current framebuffer
    // Does not modify viewport
    virtual void drawTexture(GLuint src);

    // Set draw rotation (in radians)
    // Rotation is applied before flip/scale
    void setRotation(float rad);

    // Set flip/scale for drawing; flip and scale are applied after rotation
    // eg. when x,y > 0 it's a scale; when x/y < 0 that axis is flipped
    void setFlipScale(float x, float y);

    // Special external usage: bind four vertices in counter-clockwise fan order
    void bindVertexBuffer();

    inline ProgramObject& getProgram() { return m_program; }

protected:
    ProgramObject m_program;
    // Not using the one in cgeGlobal to avoid context conflicts
    GLuint m_vertBuffer;
    GLint m_rotLoc, m_flipScaleLoc;

    void _rotate(GLint location, float rad);

protected:
    virtual CGEConstString getFragmentShaderString();
    virtual CGEConstString getVertexShaderString();
};


////////////////////////////////////////////////////////////

// Directly draw a texture with rotation, scale, and flip operations.
class TextureDrawerExt : public TextureDrawer
{
public:
    ~TextureDrawerExt();

    CGE_COMMON_CREATE_FUNC(TextureDrawerExt, init);

    // Does not handle any viewport settings; must be configured externally
    // Draw texture onto another texture
    void drawTexture2Texture(GLuint src, GLuint dst);

    // void drawTexture(GLuint src) { TextureDrawer::drawTexture(src); }

    // Bind a regular texture as the current framebuffer
    // The texture width and height must be provided to match the viewport
    void bindTextureDst(GLuint dst, int width, int height);

    // Draw texture to the current framebuffer
    // Does not modify viewport
    //  virtual void drawTexture(GLuint src);

    // Draw texture into the bound framebuffer
    void drawTexture2Buffer(GLuint src, bool fullSize = true);

    // //Set draw rotation (in radians)
    // //Rotation is applied before flip/scale
    // void setRotation(float rad);

    // //Set flip/scale for drawing; flip and scale are applied after rotation
    // //eg. when x,y > 0 it's a scale; when x/y < 0 that axis is flipped
    // void setFlipScale(float x, float y);

    // Bind four vertices in counter-clockwise fan order
    //  void bindVertexBuffer();

    CGESizei getBufferTexSize() { return m_texSize; }

protected:
    FrameBuffer m_framebuffer;
    CGESizei m_texSize;
};

class TextureDrawerWithMask : public TextureDrawer
{
public:
    CGE_COMMON_CREATE_FUNC(TextureDrawerWithMask, init);

    ~TextureDrawerWithMask();

    void setMaskRotation(float rad);
    void setMaskFlipScale(float x, float y);

    void drawTexture(GLuint src);

    void setMaskTexture(GLuint maskTexture);

protected:
    bool init();

private:
    // This will break the usage of "getFragmentShaderString" method.
    CGEConstString getFragmentShaderString() { return nullptr; }

    GLint m_maskRotLoc, m_maskFlipScaleLoc;
    GLuint m_maskTexture;
};

#ifdef GL_TEXTURE_EXTERNAL_OES

// In this class, the src texture must be of external_OES type
class TextureDrawer4ExtOES : public TextureDrawer
{
public:
    CGE_COMMON_CREATE_FUNC(TextureDrawer4ExtOES, init);

    void drawTexture(GLuint src);

    void setTransform(float* mat16);

protected:
    CGEConstString getFragmentShaderString();
    CGEConstString getVertexShaderString();

    bool init();

private:
    GLint m_transformLoc;
};

#endif

class TextureDrawerYUV : public TextureDrawer
{
public:
    CGE_COMMON_CREATE_FUNC(TextureDrawerYUV, init);

    // assume the lumaTexture is bind to GL_TEXTURE0
    // and the chromaTexture is bind to GL_TEXTURE1
    void drawTextures();
    void drawTextures(GLuint lumaTex, GLuint chromaTex);

protected:
    void drawTexture(GLuint) {}

    CGEConstString getFragmentShaderString();
    CGEConstString getVertexShaderString();

private:
    bool init();
};

}  // namespace CGE


// #undef CGE_TEXTURE_DRAWER_STATIC_FUNC

#endif
