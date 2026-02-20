/*
 * cgeSprite3d.cpp
 *
 *  Created on: 2014-10-16
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGE_ONLY_FILTERS_

#include "cgeSprite3d.h"

static CGEConstString s_vshSprite3dTest =
    CGE_SHADER_STRING(attribute vec4 v4Position; uniform mat4 mvp; varying vec3 color;

                      void main() {
                          gl_Position = mvp * v4Position;
                          color = abs(gl_Position.zzz);
                      });

static CGEConstString s_fshSprite3dTest = CGE_SHADER_STRING_PRECISION_M(

    varying vec3 color;

    void main() { gl_FragColor = vec4(color, 1.0); });

namespace CGE
{
CGEConstString SpriteInterface3d::paramAttribPositionName = "v4Position";
CGEConstString SpriteInterface3d::paramModelViewProjectionMatrixName = "mvp";
CGEConstString SpriteInterface3d::paramRotationName = "";
CGEConstString SpriteInterface3d::paramScalingName = "";
CGEConstString SpriteInterface3d::paramTranslationName = "";
CGEConstString SpriteInterface3d::paramHotspotName = "";

SpriteInterface3d::SpriteInterface3d() :
    m_pos(0.0f, 0.0f, 0.0f),
    m_scaling(1.0f, 1.0f, 1.0f),
    m_hotspot(0.0f, 0.0f, 0.0f),
    m_rotation(Mat3::makeIdentity()),
    m_alpha(1.0f),
    m_zIndex(0.0f)
{}


//////////////////////////////////////////////////////////////////////////

Sprite3dExt::~Sprite3dExt()
{
    glDeleteBuffers(1, &m_vertBuffer.bufferID);
    glDeleteBuffers(1, &m_vertElementBuffer.bufferID);
}

bool Sprite3dExt::init(const ArrayBufferComponent& vertexBuffer, const ArrayBufferComponent& vertexElementArrayBuffer,
                       GLenum drawFunc)
{
    m_vertBuffer = vertexBuffer;
    m_vertElementBuffer = vertexElementArrayBuffer;

    if (m_vertBuffer.bufferID == 0)
    {
        m_vertBuffer.bufferID = genBufferWithComponent(m_vertBuffer);
    }

    if (vertexElementArrayBuffer.bufferID == 0)
    {
        m_vertElementBuffer.bufferID = genBufferWithComponent(m_vertElementBuffer);
    }

    cgeCheckGLError("Sprite3d::init");
    m_vertBuffer.bufferData = nullptr;
    m_vertElementBuffer.bufferData = nullptr;
    m_drawFunc = drawFunc;
    return _initProgram(s_vshSprite3dTest, s_fshSprite3dTest);
}

void Sprite3dExt::_bindProgramLocations()
{
    m_vertAttribLocation = 0;
    m_program.bindAttribLocation(paramAttribPositionName, m_vertAttribLocation);
}

void Sprite3dExt::_initProgramUniforms()
{
    m_program.bind();
    m_mvpLocation = m_program.uniformLocation(paramModelViewProjectionMatrixName);
}

bool Sprite3dExt::_initProgram(CGEConstString vsh, CGEConstString fsh)
{
    _bindProgramLocations();

    if (!m_program.initWithShaderStrings(vsh, fsh))
    {
        CGE_LOG_ERROR("Sprite3dExt::_initProgram Failed!");
        return false;
    }
    _initProgramUniforms();
    return true;
}

void Sprite3dExt::renderWithMat(const Mat4& modelViewProjectionMatrix)
{
    Mat4 mvp = modelViewProjectionMatrix *
        (Mat4(m_scaling[0], 0.0f, 0.0f, 0.0f, 0.0f, m_scaling[1], 0.0f, 0.0f, 0.0f, 0.0f, m_scaling[2], 0.0f, m_pos[0],
              m_pos[1], m_pos[2], 1.0f) *
         m_rotation);

    m_program.bind();
    glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, mvp[0]);
    glBindBuffer(m_vertBuffer.bufferKind, m_vertBuffer.bufferID);
    glEnableVertexAttribArray(m_vertAttribLocation);
    glVertexAttribPointer(m_vertAttribLocation, m_vertBuffer.componentSize, m_vertBuffer.bufferDataKind, GL_FALSE,
                          m_vertBuffer.bufferStride, 0);

    glBindBuffer(m_vertElementBuffer.bufferKind, m_vertElementBuffer.bufferID);

    glDrawElements(m_drawFunc, m_vertElementBuffer.elementCnt, m_vertElementBuffer.bufferDataKind, 0);

    cgeCheckGLError("Sprite3dExt::renderWithMat");
}


}  // namespace CGE

#endif