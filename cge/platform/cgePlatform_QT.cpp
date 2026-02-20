/*
 * cgePlatforms.h
 *
 *  Created on: 2014-6-8
 *      Author: Wang Yang
 *  Description: be compatible with none-angle qt versions.
 */


#if !defined(QT_OPENGL_ES_2) || defined(Q_OS_MAC)

#include "cgePlatform_QT.h"

namespace CGE4QT_OpenGLFunctions
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QOpenGLFunctions* g_glFunctions = nullptr;
#else
QGLFunctions* g_glFunctions = nullptr;
#endif
}  // namespace CGE4QT_OpenGLFunctions
#endif
