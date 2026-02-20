//
//  cgeDistortionFilter.cpp
//  cgeStatic
//
//  Created by wangyang on 16/6/1.
//  Copyright © 2016 wysaid. All rights reserved.
//

#include "cgeDistortionFilter.h"
#include <cmath>

static CGEConstString s_fshBloatWrinkle = CGE_SHADER_STRING_PRECISION_M(
    varying vec2 textureCoordinate; uniform sampler2D inputImageTexture;

    uniform float intensity; uniform float radius; uniform vec2 imageSteps; uniform vec2 keyPoint;

    void main() {
        vec2 realPosition = textureCoordinate / imageSteps;
        vec2 dis = keyPoint - realPosition;
        float len = length(dis);
        vec2 offset;

        if (len < radius)
        {
            float fac = 1.0 - len / radius;
            offset = intensity * dis * imageSteps * fac * fac;
        }
        else
        {
            offset = vec2(0.0);
        }

        gl_FragColor = texture2D(inputImageTexture, textureCoordinate + offset);
    });

static CGEConstString s_fshDistortionMove = CGE_SHADER_STRING_PRECISION_H(
    varying vec2 textureCoordinate; uniform sampler2D inputImageTexture;

    uniform float intensity; uniform float radius; uniform vec2 imageSteps; uniform vec2 keyPoint;
    uniform vec2 keyPoint2;

    // Line equation coefficients; the line is defined by a start and end point.
    uniform float equation[5];  // In order: eqA, eqB, eqC, eqD, eqD2;

    void main() {
        vec2 realPosition = textureCoordinate / imageSteps;
        vec2 offset = vec2(0.0);

        // Distance from point to line
        float disToLine = abs(equation[0] * realPosition.x + equation[1] * realPosition.y + equation[2]) / equation[3];

        if (disToLine < radius)
        {
            float eqAB = equation[0] * equation[1];

            vec2 v2Min = min(keyPoint, keyPoint2);
            vec2 v2Max = max(keyPoint, keyPoint2);

            // Projection of the point onto the line direction
            vec2 projVec =
                vec2((equation[1] * equation[1] * realPosition.x - eqAB * realPosition.y - equation[0] * equation[2]) /
                         equation[4],
                     (equation[0] * equation[0] * realPosition.y - eqAB * realPosition.x - equation[1] * equation[2]) /
                         equation[4]);

            // Compute actual distance: use point-to-line distance if projection is on segment, otherwise use nearest
            // endpoint distance
            float percent;

            // Projection onto the line is outside the segment
            if (projVec.x < v2Min.x || projVec.x > v2Max.x || projVec.y < v2Min.y || projVec.y > v2Max.y)
            {
                // Check if within radius of the nearest endpoint
                float dis2 = length(realPosition - keyPoint);
                float dis3 = length(realPosition - keyPoint2);

                if (dis2 < radius || dis3 < radius)
                {
                    percent = (1.0 - min(dis2, dis3) / radius);
                }
                else
                {
                    percent = 0.0;
                }
            }
            else
            {
                percent = (1.0 - disToLine / radius);
            }

            percent = smoothstep(0.0, 1.0, percent) * intensity;

            offset += (keyPoint - keyPoint2) * imageSteps * percent;
        }

        gl_FragColor = texture2D(inputImageTexture, textureCoordinate + offset);
    });

namespace CGE
{
CGEDistortionFilter::CGEDistortionFilter() :
    m_intensityLoc(-1), m_radiusLoc(-1), m_stepsLoc(-1), m_keyPointLoc(-1), m_keyPoint2Loc(-1), m_equationLoc(-1)
{}

CGEDistortionFilter::~CGEDistortionFilter()
{}

bool CGEDistortionFilter::initDistortionBloatWrinkle()
{
    return _setup(g_vshDefaultWithoutTexCoord, s_fshBloatWrinkle);
}

bool CGEDistortionFilter::initDistortionForward()
{
    return _setup(g_vshDefaultWithoutTexCoord, s_fshDistortionMove);
}

void CGEDistortionFilter::setIntensity(float value)
{
    m_program.bind();
    glUniform1f(m_intensityLoc, value);
    m_intensity = value;
}

void CGEDistortionFilter::setSteps(float x, float y)
{
    m_program.bind();
    glUniform2f(m_stepsLoc, x, y);
}

void CGEDistortionFilter::setPointParams(float pointX, float pointY, float radius, float intensity)
{
    m_program.bind();
    glUniform2f(m_keyPointLoc, pointX, pointY);
    glUniform1f(m_radiusLoc, radius);
    if (intensity >= 0.0f)
    {
        glUniform1f(m_intensityLoc, m_intensity * intensity);
    }
}

void CGEDistortionFilter::setForwardParams(float point1X, float point1Y, float point2X, float point2Y, float radius,
                                           float intensity)
{
    m_program.bind();
    glUniform2f(m_keyPointLoc, point1X, point1Y);
    glUniform2f(m_keyPoint2Loc, point2X, point2Y);
    glUniform1f(m_radiusLoc, radius);

    // Line equation coefficients; the line is defined by a start and end point.

    float equation[5];
    float &eqA = equation[0], &eqB = equation[1], &eqC = equation[2], &eqD = equation[3], &eqD2 = equation[4];

    // Compute line equation
    {
        float a = point1Y - point2Y, b = point1X - point2X, c = point1X * point2Y - point1Y * point2X;

        if (CGE_FLOATCOMP0(b))
        {
            eqA = 1.0f;
            eqB = 0.0f;
            eqC = -point1X;
        }
        else
        {
            eqA = a / b;
            eqB = -1.0f;
            eqC = c / b;
        }
        eqD2 = eqA * eqA + eqB * eqB;
        eqD = sqrtf(eqD2);
    }

    glUniform1fv(m_equationLoc, 5, equation);

    if (intensity >= 0.0f)
    {
        glUniform1f(m_intensityLoc, m_intensity * intensity);
    }
}

bool CGEDistortionFilter::_setup(CGEConstString vsh, CGEConstString fsh)
{
    if (m_program.initWithShaderStrings(vsh, fsh))
    {
        m_program.bind();

        m_intensityLoc = m_program.uniformLocation("intensity");
        m_radiusLoc = m_program.uniformLocation("radius");
        m_stepsLoc = m_program.uniformLocation("imageSteps");
        m_keyPointLoc = m_program.uniformLocation("keyPoint");
        m_keyPoint2Loc = m_program.uniformLocation("keyPoint2");
        m_equationLoc = m_program.uniformLocation("equation");

        glUniform1f(m_intensityLoc, 1.0f);
        return true;
    }

    return false;
}

}  // namespace CGE
