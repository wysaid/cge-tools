/*
 * cgeEnlargeEyeFilter.cpp
 *
 *  Created on: 2014-4-23
 *      Author: Wang Yang
 */

#include "cgeEnlargeEyeFilter.h"

static CGEConstString s_fshEnlargeEye =
    CGE_SHADER_STRING_PRECISION_M(varying vec2 textureCoordinate; uniform sampler2D inputImageTexture;

                                  uniform vec2 samplerSteps; uniform vec2 centralPos;  // Real position for pixels.
                                  uniform float radius;                                // Range: [1.0, 1000.0]
                                  uniform float intensity;                             // Range: [-1.0, 1.0]

                                  void main() {
                                      vec2 realPosition = textureCoordinate / samplerSteps;
                                      vec2 dis = centralPos - realPosition;
                                      float len = length(dis);
                                      if (len > radius)
                                      {
                                          gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
                                          return;
                                      }

                                      float offset = 1.0 - len / radius;
                                      // smoothstep:
                                      offset = offset * offset * (3.0 - 2.0 * offset);
                                      vec2 step = intensity * dis * samplerSteps * offset;
                                      gl_FragColor = texture2D(inputImageTexture, textureCoordinate + step);
                                  });

static CGEConstString s_fshEnlarge2Eyes = CGE_SHADER_STRING_PRECISION_M(
    varying vec2 textureCoordinate; uniform sampler2D inputImageTexture;

    uniform vec2 samplerSteps; uniform vec2 leftEyePos;  // Real position for pixels.
    uniform vec2 rightEyePos; uniform vec2 radius;       // Range: [1.0, 1000.0]
    uniform float intensity;                             // Range: [-1.0, 1.0]

    void main() {
        float offset;
        vec2 step;

        vec2 realPosition = textureCoordinate / samplerSteps;

        vec2 dis = leftEyePos - realPosition;
        float len = length(dis);

        if (len < radius.x)
        {
            offset = 1.0 - len / radius.x;
            offset = smoothstep(0.0, 1.0, offset);  // offset * offset * (3.0 - 2.0 * offset);
            step = intensity * dis * samplerSteps * offset;
            gl_FragColor = texture2D(inputImageTexture, textureCoordinate + step);
            return;
        }

        dis = rightEyePos - realPosition;
        len = length(dis);

        if (len < radius.y)
        {
            offset = 1.0 - len / radius.y;
            offset = smoothstep(0.0, 1.0, offset);  // offset * offset * (3.0 - 2.0 * offset);
            step = intensity * dis * samplerSteps * offset;
            gl_FragColor = texture2D(inputImageTexture, textureCoordinate + step);
            return;
        }

        gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
    });

static CGEConstString s_fshEnlarge2EyesAndMouth =
    CGE_SHADER_STRING_PRECISION_M(varying vec2 textureCoordinate; uniform sampler2D inputImageTexture;

                                  uniform vec2 samplerSteps; uniform vec2 leftEyePos; uniform vec2 rightEyePos;
                                  uniform vec2 radius; uniform float intensity;

                                  uniform vec2 mouthPos; uniform float mouthRadius;

                                  void main() {
                                      float offset;
                                      vec2 step;

                                      vec2 realPosition = textureCoordinate / samplerSteps;

                                      vec2 dis = leftEyePos - realPosition;
                                      float len = length(dis);

                                      if (len < radius.x)
                                      {
                                          offset = 1.0 - len / radius.x;
                                          offset = smoothstep(0.0, 1.0, offset);
                                          step = intensity * dis * samplerSteps * offset;
                                          gl_FragColor = texture2D(inputImageTexture, textureCoordinate + step);
                                          return;
                                      }

                                      dis = rightEyePos - realPosition;
                                      len = length(dis);

                                      if (len < radius.y)
                                      {
                                          offset = 1.0 - len / radius.y;
                                          offset = smoothstep(0.0, 1.0, offset);
                                          step = intensity * dis * samplerSteps * offset;
                                          gl_FragColor = texture2D(inputImageTexture, textureCoordinate + step);
                                          return;
                                      }

                                      dis = mouthPos - realPosition;
                                      len = length(dis);

                                      if (len < mouthRadius)
                                      {
                                          offset = 1.0 - len / mouthRadius;
                                          offset = smoothstep(0.0, 1.0, offset);
                                          step = intensity * dis * samplerSteps * offset;
                                          gl_FragColor = texture2D(inputImageTexture, textureCoordinate + step);
                                          return;
                                      }

                                      gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
                                  });


namespace CGE
{
CGEConstString CGEEnlargeEyeFilter::paramRadiusName = "radius";
CGEConstString CGEEnlargeEyeFilter::paramIntensityName = "intensity";
CGEConstString CGEEnlargeEyeFilter::paramCentralPosName = "centralPos";

bool CGEEnlargeEyeFilter::init()
{
    if (initShadersFromString(g_vshDefaultWithoutTexCoord, s_fshEnlargeEye))
    {
        setCentralPosition(100.0f, 100.0f);
        setEnlargeRadius(100.0f);
        setIntensity(1.0f);
        return true;
    }
    return false;
}

void CGEEnlargeEyeFilter::setEnlargeRadius(float radius)
{
    m_program.bind();
    m_program.sendUniformf(paramRadiusName, radius);
}

void CGEEnlargeEyeFilter::setIntensity(float value)
{
    m_program.bind();
    m_program.sendUniformf(paramIntensityName, value);
}

void CGEEnlargeEyeFilter::setCentralPosition(float x, float y)
{
    m_program.bind();
    m_program.sendUniformf(paramCentralPosName, x, y);
}


/////////////////////////////////////////////

CGEConstString CGEEnlarge2EyesFilter::paramEyeRadiusName = "radius";
CGEConstString CGEEnlarge2EyesFilter::paramIntensityName = "intensity";
CGEConstString CGEEnlarge2EyesFilter::paramLeftEyePosName = "leftEyePos";
CGEConstString CGEEnlarge2EyesFilter::paramRightEyePosName = "rightEyePos";

bool CGEEnlarge2EyesFilter::init()
{
    if (initShadersFromString(g_vshDefaultWithoutTexCoord, s_fshEnlarge2Eyes))
    {
        setEyeEnlargeRadius(100.0f, 100.0f);
        setIntensity(1.0f);
        return true;
    }
    return false;
}

void CGEEnlarge2EyesFilter::setEyeEnlargeRadius(float leftEyeRadius, float rightEyeRadius)
{
    m_program.bind();
    m_program.sendUniformf(paramEyeRadiusName, leftEyeRadius, rightEyeRadius);
}

void CGEEnlarge2EyesFilter::setIntensity(float value)
{
    m_program.bind();
    m_program.sendUniformf(paramIntensityName, value);
}

void CGEEnlarge2EyesFilter::setEyePos(const Vec2f& left, const Vec2f& right)
{
    m_program.bind();
    m_program.sendUniformf(paramLeftEyePosName, left[0], left[1]);
    m_program.sendUniformf(paramRightEyePosName, right[0], right[1]);
}

/////////////////////////////////////////////

CGEConstString CGEEnlarge2EyesAndMouthFilter::paramMouthRadiusName = "mouthRadius";
CGEConstString CGEEnlarge2EyesAndMouthFilter::paramMouthPosName = "mouthPos";

bool CGEEnlarge2EyesAndMouthFilter::init()
{
    if (initShadersFromString(g_vshDefaultWithoutTexCoord, s_fshEnlarge2EyesAndMouth))
    {
        setEyeEnlargeRadius(100.0f, 100.0f);
        setIntensity(1.0f);
        setMouthEnlargeRadius(100.0f);
        return true;
    }
    return false;
}

void CGEEnlarge2EyesAndMouthFilter::setMouthEnlargeRadius(float mouthRadius)
{
    m_program.bind();
    m_program.sendUniformf(paramMouthRadiusName, mouthRadius);
}

void CGEEnlarge2EyesAndMouthFilter::setMouthPos(const Vec2f& pos)
{
    m_program.bind();
    m_program.sendUniformf(paramMouthPosName, pos[0], pos[1]);
}

}  // namespace CGE
