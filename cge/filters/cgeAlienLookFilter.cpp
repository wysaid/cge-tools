/* cgeAlienLookFilter.cpp
 *
 *  Created on: 2016-3-23
 *      Author: Wang Yang
 */


#include "cgeAlienLookFilter.h"
#include <cmath>

CGEConstString s_fshAlienLook = CGE_SHADER_STRING_PRECISION_H(
    varying vec2 textureCoordinate; uniform sampler2D inputImageTexture; uniform float intensity;

    uniform vec2 imageSteps; uniform vec2 leftEye; uniform vec2 rightEye; uniform vec2 mouth;


    void main() {
        vec2 realPosition = textureCoordinate / imageSteps;
        vec2 eyeDir = leftEye - rightEye;

        float halfEyeDis = length(eyeDir) * 0.7;

        vec2 offset = vec2(0.0);
        float fac;

        vec2 dis = leftEye - realPosition;
        float len = length(dis);

        if (len < halfEyeDis)
        {
            fac = 1.0 - len / halfEyeDis;
            offset = intensity * dis * imageSteps * fac * fac;
        }

        dis = rightEye - realPosition;
        len = length(dis);
        if (len < halfEyeDis)
        {
            fac = 1.0 - len / halfEyeDis;
            offset += intensity * dis * imageSteps * fac * fac;
        }

        float mouthDis = length((leftEye + rightEye) / 2.0 - mouth);
        dis = mouth - realPosition;
        // float scaling = length(dis * normalize(eyeDir));
        len = length(dis);
        if (len < mouthDis)
        {
            fac = len / mouthDis - 1.0;
            // fac *= scaling;
            offset += intensity * dis * imageSteps * fac * fac * fac;
        }

        gl_FragColor = texture2D(inputImageTexture, textureCoordinate + offset);
    });

namespace CGE
{
CGEConstString CGEAlienLookFilter::paramLeftEye = "leftEye";
CGEConstString CGEAlienLookFilter::paramRightEye = "rightEye";
CGEConstString CGEAlienLookFilter::paramMouth = "mouth";

bool CGEAlienLookFilter::init()
{
    if (initShadersFromString(g_vshDefaultWithoutTexCoord, s_fshAlienLook))
    {
        m_program.bind();
        m_leftEyeLoc = m_program.uniformLocation(paramLeftEye);
        m_rightEyeLoc = m_program.uniformLocation(paramRightEye);
        m_mouthLoc = m_program.uniformLocation(paramMouth);
        setImageSize(720.0f, 1280.0f);
        setIntensity(1.0f);
        return true;
    }
    return false;
}

void CGEAlienLookFilter::setIntensity(float value)
{
    m_program.bind();
    m_program.sendUniformf("intensity", value);
}

// void CGEAlienLookFilter::render2Texture(CGEImageHandlerInterface* handler, GLuint srcTexture, GLuint vertexBufferID)
// {

// }

void CGEAlienLookFilter::setImageSize(float width, float height)
{
    m_program.bind();
    m_program.sendUniformf("imageSteps", 1.0f / width, 1.0f / height);
}

void CGEAlienLookFilter::updateKeyPoints(float leftEyeX, float leftEyeY, float rightEyeX, float rightEyeY, float mouthX,
                                         float mouthY)
{
    m_program.bind();
    glUniform2f(m_leftEyeLoc, leftEyeX, leftEyeY);
    glUniform2f(m_rightEyeLoc, rightEyeX, rightEyeY);
    glUniform2f(m_mouthLoc, mouthX, mouthY);
}


}  // namespace CGE
