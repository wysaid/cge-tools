/*
 * cgeGeometryUtils.cpp
 *
 *  Created on: 2017-4-15
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#include "cgeGeometryUtils.h"

static CGEConstString s_vshPoint = CGE_SHADER_STRING(attribute vec2 position; uniform float pointSize;

                                                     void main() {
                                                         gl_PointSize = pointSize;
                                                         gl_Position = vec4(position, 0.0, 1.0);
                                                     }

);

static CGEConstString s_fshPoint = CGE_SHADER_STRING_PRECISION_L(uniform vec4 color;

                                                                 void main() { gl_FragColor = color; });

#define VSH_POSITION_NAME "position"
#define VSH_POINT_SIZE_NAME "pointSize"
#define VSH_COLOR "color"

namespace CGE
{
CGEPointDrawer::CGEPointDrawer()
{
    m_program.bindAttribLocation(VSH_POSITION_NAME, 0);
}

bool CGEPointDrawer::init()
{
    if (m_program.initWithShaderStrings(getVSH(), getFSH()))
    {
        m_program.bind();
        m_pointSizeLoc = m_program.uniformLocation(VSH_POINT_SIZE_NAME);
        glUniform1f(m_pointSizeLoc, 1.0f);
        m_colorLoc = m_program.uniformLocation(VSH_COLOR);
        return true;
    }

    return false;
}

void CGEPointDrawer::draw(Vec2f* points, int count, int stride)
{
    m_program.bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, points);
    glDrawArrays(GL_POINTS, 0, count);
}

void CGEPointDrawer::setPointSize(float pointSize)
{
    m_program.bind();
    glUniform1f(m_pointSizeLoc, pointSize);
}

void CGEPointDrawer::setColor(Vec4f color)
{
    m_program.bind();
    glUniform4f(m_colorLoc, color[0], color[1], color[2], color[3]);
}

CGEConstString CGEPointDrawer::getVSH()
{
    return s_vshPoint;
}

CGEConstString CGEPointDrawer::getFSH()
{
    return s_fshPoint;
}

}  // namespace CGE