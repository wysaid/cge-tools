/*
 * cgePlatforms.h
 *
 *  Created on: 2013-12-31
 *      Author: Wang Yang
 *  Description: load some library and do some essential initialization before compiling.
 */

#ifndef _CGEPLATFORMS_H_
#define _CGEPLATFORMS_H_

#include <QtCore/QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#elif defined(QT_OPENGL_ES_2)
#include <QOpenGLFunctions_ES2>
#include <qgl.h>
#else
#include <QGLFunctions>
#include <qgl.h>
#endif

#if defined(_CGE_GENERAL_ERROR_TEST_) && _CGE_GENERAL_ERROR_TEST_
#define CGE_UNEXPECTED_ERR_MSG(...) __VA_ARGS__
#else
#define CGE_UNEXPECTED_ERR_MSG(...)
#endif

#if defined(_MSC_VER)

// #if _MSC_VER > 1600 && !defined(_CRT_SECURE_NO_DEPRECATE)
// #define _CRT_SECURE_NO_DEPRECATE 1
// #endif

#if defined(DEBUG) || defined(_DEBUG) || defined(CGE_KEEP_LOG_INFO) || defined(CGE_KEEP_LOG_ERROR)

#include <cstdio>
#include <windows.h>

#define F_RED FOREGROUND_INTENSITY | FOREGROUND_RED
#define F_BLUE FOREGROUND_INTENSITY | FOREGROUND_BLUE
#define F_GREEN FOREGROUND_INTENSITY | FOREGROUND_GREEN
#define F_YELLOW FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define F_PURPLE FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define F_CYAN FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE

#define B_RED BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED
#define B_BLUE BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_BLUE
#define B_GREEN BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_GREEN
#define B_YELLOW BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN
#define B_PURPLE BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE
#define B_CYAN BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE

#endif

#if defined(DEBUG) || defined(_DEBUG)

#ifdef CGE_NO_LOG_INFO
#define CGE_LOG_INFO(...)
#endif

#ifdef CGE_NO_LOG_CODE
#define CGE_LOG_CODE(...)
#endif

#ifdef CGE_NO_LOG_ERROR
#define CGE_LOG_ERROR(...)
#endif

#else

#ifndef CGE_KEEP_LOG_INFO
#define CGE_LOG_INFO(...)
#endif

#ifndef CGE_KEEP_LOG_CODE
#define CGE_LOG_CODE(...)
#endif

#ifndef CGE_KEEP_LOG_ERROR
#define CGE_LOG_ERROR(...)
#endif

#endif

#ifndef CGE_LOG_INFO
#define CGE_LOG_INFO(...)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
        printf(__VA_ARGS__);                                                                                           \
        fflush(stdout);                                                                                                \
    } while (0)
#endif

#ifndef CGE_LOG_CODE
#define CGE_LOG_CODE(...) __VA_ARGS__
#endif

#ifndef CGE_LOG_ERROR
#define CGE_LOG_ERROR(...)                                                         \
    do                                                                             \
    {                                                                              \
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), F_YELLOW | B_RED); \
        fprintf(stderr, __VA_ARGS__);                                              \
        fflush(stderr);                                                            \
    } while (0)

#endif

#elif defined(Q_OS_MACOS)

#define _CGE_SHADER_VERSION_ 120

#if (defined(DEBUG) || defined(_DEBUG))
#include <cstdio>
#define CGE_LOG_ERROR(str, ...)                         \
    do                                                  \
    {                                                   \
        fprintf(stderr, "❌❌❌");                      \
        fprintf(stderr, str, ##__VA_ARGS__);            \
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); \
        fprintf(stderr, "❌❌❌\n");                    \
        fflush(stderr);                                 \
    } while (0)

#define CGE_LOG_CODE(...) __VA_ARGS__
#define CGE_LOG_INFO(...)    \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    } while (0)

#else

#define CGE_LOG_ERROR(...)
#define CGE_LOG_CODE(...)
#define CGE_LOG_INFO(...)

#endif

#elif defined(Q_OS_ANDROID)

#include <android/log.h>

#ifndef CGE_LOG_TAG
#define CGE_LOG_TAG "CGE"
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define CGE_LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, CGE_LOG_TAG, __VA_ARGS__)
#define CGE_LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, CGE_LOG_TAG, __VA_ARGS__)
#define CGE_LOG_CODE(...) __VA_ARGS__
#else
#define CGE_LOG_INFO(...)
#define CGE_LOG_ERROR(...)
#define CGE_LOG_CODE(...)
#endif


#elif defined(Q_OS_IOS)

#if (defined(DEBUG) || defined(_DEBUG))
#include <cstdio>
#define CGE_LOG_ERROR(str, ...)                         \
    do                                                  \
    {                                                   \
        fprintf(stderr, "❌❌❌");                      \
        fprintf(stderr, str, ##__VA_ARGS__);            \
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); \
        fprintf(stderr, "❌❌❌\n");                    \
        fflush(stderr);                                 \
    } while (0)

#define CGE_LOG_CODE(...) __VA_ARGS__
#define CGE_LOG_INFO(...)    \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    } while (0)

#else

#define CGE_LOG_ERROR(...)
#define CGE_LOG_CODE(...)
#define CGE_LOG_INFO(...)

#endif


#else


#if (defined(DEBUG) || defined(_DEBUG))
#include <cstdio>
#define CGE_LOG_ERROR(str, ...)                         \
    do                                                  \
    {                                                   \
        fprintf(stderr, "❌❌❌");                      \
        fprintf(stderr, str, ##__VA_ARGS__);            \
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); \
        fprintf(stderr, "❌❌❌\n");                    \
        fflush(stderr);                                 \
    } while (0)

#define CGE_LOG_CODE(...) __VA_ARGS__
#define CGE_LOG_INFO(...)    \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        fflush(stdout);      \
    } while (0)

#else

#define CGE_LOG_ERROR(...)
#define CGE_LOG_CODE(...)
#define CGE_LOG_INFO(...)

#endif

#endif

#if !defined(QT_OPENGL_ES_2) && !defined(Q_OS_MAC)

#define CGE_USE_COMPATIBLE_GL_FUNCTIONS

namespace CGE4QT_OpenGLFunctions
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
extern QOpenGLFunctions* g_glFunctions;
#else
extern QGLFunctions* g_glFunctions;
#endif

inline void glActiveTexture(GLenum texture)
{
    return g_glFunctions->glActiveTexture(texture);
}

inline void glAttachShader(GLuint program, GLuint shader)
{
    return g_glFunctions->glAttachShader(program, shader);
}

inline void glBindAttribLocation(GLuint program, GLuint index, const char* name)
{
    return g_glFunctions->glBindAttribLocation(program, index, name);
}

inline void glBindBuffer(GLenum target, GLuint buffer)
{
    return g_glFunctions->glBindBuffer(target, buffer);
}

inline void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    return g_glFunctions->glBindFramebuffer(target, framebuffer);
}

inline void glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    return g_glFunctions->glBindRenderbuffer(target, renderbuffer);
}

inline void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    return g_glFunctions->glBlendColor(red, green, blue, alpha);
}

inline void glBlendEquation(GLenum mode)
{
    return g_glFunctions->glBlendEquation(mode);
}

inline void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    return g_glFunctions->glBlendEquationSeparate(modeRGB, modeAlpha);
}

inline void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    return g_glFunctions->glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

inline void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
    return g_glFunctions->glBufferData(target, size, data, usage);
}

inline void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
    return g_glFunctions->glBufferSubData(target, offset, size, data);
}

inline GLenum glCheckFramebufferStatus(GLenum target)
{
    return g_glFunctions->glCheckFramebufferStatus(target);
}

inline void glClearDepthf(GLclampf depth)
{
    return g_glFunctions->glClearDepthf(depth);
}

inline void glCompileShader(GLuint shader)
{
    return g_glFunctions->glCompileShader(shader);
}

inline void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height,
                                   GLint border, GLsizei imageSize, const void* data)
{
    return g_glFunctions->glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

inline void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
                                      GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
    return g_glFunctions->glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize,
                                                    data);
}

inline GLuint glCreateProgram()
{
    return g_glFunctions->glCreateProgram();
}

inline GLuint glCreateShader(GLenum type)
{
    return g_glFunctions->glCreateShader(type);
}

inline void glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
    return g_glFunctions->glDeleteBuffers(n, buffers);
}

inline void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
    return g_glFunctions->glDeleteFramebuffers(n, framebuffers);
}

inline void glDeleteProgram(GLuint program)
{
    return g_glFunctions->glDeleteProgram(program);
}

inline void glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
    return g_glFunctions->glDeleteRenderbuffers(n, renderbuffers);
}

inline void glDeleteShader(GLuint shader)
{
    return g_glFunctions->glDeleteShader(shader);
}

inline void glDepthRangef(GLclampf zNear, GLclampf zFar)
{
    return g_glFunctions->glDepthRangef(zNear, zFar);
}

inline void glDetachShader(GLuint program, GLuint shader)
{
    return g_glFunctions->glDetachShader(program, shader);
}

inline void glDisableVertexAttribArray(GLuint index)
{
    return g_glFunctions->glDisableVertexAttribArray(index);
}

inline void glEnableVertexAttribArray(GLuint index)
{
    return g_glFunctions->glEnableVertexAttribArray(index);
}

inline void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    return g_glFunctions->glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

inline void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    return g_glFunctions->glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

inline void glGenBuffers(GLsizei n, GLuint* buffers)
{
    return g_glFunctions->glGenBuffers(n, buffers);
}

inline void glGenerateMipmap(GLenum target)
{
    return g_glFunctions->glGenerateMipmap(target);
}

inline void glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
    return g_glFunctions->glGenFramebuffers(n, framebuffers);
}

inline void glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
    return g_glFunctions->glGenRenderbuffers(n, renderbuffers);
}

inline void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type,
                              char* name)
{
    return g_glFunctions->glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

inline void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size,
                               GLenum* type, char* name)
{
    return g_glFunctions->glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

inline void glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
    return g_glFunctions->glGetAttachedShaders(program, maxcount, count, shaders);
}

inline GLint glGetAttribLocation(GLuint program, const char* name)
{

    return g_glFunctions->glGetAttribLocation(program, name);
}

inline void glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    return g_glFunctions->glGetBufferParameteriv(target, pname, params);
}

inline void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
    return g_glFunctions->glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

inline void glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    return g_glFunctions->glGetProgramiv(program, pname, params);
}

inline void glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
    return g_glFunctions->glGetProgramInfoLog(program, bufsize, length, infolog);
}

inline void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    return g_glFunctions->glGetRenderbufferParameteriv(target, pname, params);
}

inline void glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    return g_glFunctions->glGetShaderiv(shader, pname, params);
}

inline void glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
    return g_glFunctions->glGetShaderInfoLog(shader, bufsize, length, infolog);
}

inline void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
    return g_glFunctions->glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

inline void glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
    return g_glFunctions->glGetShaderSource(shader, bufsize, length, source);
}

inline void glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
    return g_glFunctions->glGetUniformfv(program, location, params);
}

inline void glGetUniformiv(GLuint program, GLint location, GLint* params)
{
    return g_glFunctions->glGetUniformiv(program, location, params);
}

inline GLint glGetUniformLocation(GLuint program, const char* name)
{
    return g_glFunctions->glGetUniformLocation(program, name);
}

inline void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
    return g_glFunctions->glGetVertexAttribfv(index, pname, params);
}

inline void glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
    return g_glFunctions->glGetVertexAttribiv(index, pname, params);
}

inline void glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer)
{
    return g_glFunctions->glGetVertexAttribPointerv(index, pname, pointer);
}

inline GLboolean glIsBuffer(GLuint buffer)
{
    return g_glFunctions->glIsBuffer(buffer);
}

inline GLboolean glIsFramebuffer(GLuint framebuffer)
{
    return g_glFunctions->glIsFramebuffer(framebuffer);
}

inline GLboolean glIsProgram(GLuint program)
{
    return g_glFunctions->glIsProgram(program);
}

inline GLboolean glIsRenderbuffer(GLuint renderbuffer)
{
    return g_glFunctions->glIsRenderbuffer(renderbuffer);
}

inline GLboolean glIsShader(GLuint shader)
{
    return g_glFunctions->glIsShader(shader);
}

inline void glLinkProgram(GLuint program)
{
    return g_glFunctions->glLinkProgram(program);
}

inline void glReleaseShaderCompiler()
{
    return g_glFunctions->glReleaseShaderCompiler();
}

inline void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    return g_glFunctions->glRenderbufferStorage(target, internalformat, width, height);
}

inline void glSampleCoverage(GLclampf value, GLboolean invert)
{
    return g_glFunctions->glSampleCoverage(value, invert);
}

inline void glShaderBinary(GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length)
{
    return g_glFunctions->glShaderBinary(n, shaders, binaryformat, binary, length);
}

inline void glShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length)
{
    return g_glFunctions->glShaderSource(shader, count, string, length);
}

inline void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    return g_glFunctions->glStencilFuncSeparate(face, func, ref, mask);
}

inline void glStencilMaskSeparate(GLenum face, GLuint mask)
{
    return g_glFunctions->glStencilMaskSeparate(face, mask);
}

inline void glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
    return g_glFunctions->glStencilOpSeparate(face, fail, zfail, zpass);
}

inline void glUniform1f(GLint location, GLfloat x)
{
    return g_glFunctions->glUniform1f(location, x);
}

inline void glUniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
    return g_glFunctions->glUniform1fv(location, count, v);
}

inline void glUniform1i(GLint location, GLint x)
{
    return g_glFunctions->glUniform1i(location, x);
}

inline void glUniform1iv(GLint location, GLsizei count, const GLint* v)
{
    return g_glFunctions->glUniform1iv(location, count, v);
}

inline void glUniform2f(GLint location, GLfloat x, GLfloat y)
{
    return g_glFunctions->glUniform2f(location, x, y);
}

inline void glUniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
    return g_glFunctions->glUniform2fv(location, count, v);
}

inline void glUniform2i(GLint location, GLint x, GLint y)
{
    return g_glFunctions->glUniform2i(location, x, y);
}

inline void glUniform2iv(GLint location, GLsizei count, const GLint* v)
{
    return g_glFunctions->glUniform2iv(location, count, v);
}

inline void glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
    return g_glFunctions->glUniform3f(location, x, y, z);
}

inline void glUniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
    return g_glFunctions->glUniform3fv(location, count, v);
}

inline void glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
    return g_glFunctions->glUniform3i(location, x, y, z);
}

inline void glUniform3iv(GLint location, GLsizei count, const GLint* v)
{
    return g_glFunctions->glUniform3iv(location, count, v);
}

inline void glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    return g_glFunctions->glUniform4f(location, x, y, z, w);
}

inline void glUniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
    return g_glFunctions->glUniform4fv(location, count, v);
}

inline void glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
    return g_glFunctions->glUniform4i(location, x, y, z, w);
}

inline void glUniform4iv(GLint location, GLsizei count, const GLint* v)
{
    return g_glFunctions->glUniform4iv(location, count, v);
}

inline void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    return g_glFunctions->glUniformMatrix2fv(location, count, transpose, value);
}

inline void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    return g_glFunctions->glUniformMatrix3fv(location, count, transpose, value);
}

inline void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    return g_glFunctions->glUniformMatrix4fv(location, count, transpose, value);
}

inline void glUseProgram(GLuint program)
{
    return g_glFunctions->glUseProgram(program);
}

inline void glValidateProgram(GLuint program)
{
    return g_glFunctions->glValidateProgram(program);
}

inline void glVertexAttrib1f(GLuint indx, GLfloat x)
{
    return g_glFunctions->glVertexAttrib1f(indx, x);
}

inline void glVertexAttrib1fv(GLuint indx, const GLfloat* values)
{
    return g_glFunctions->glVertexAttrib1fv(indx, values);
}

inline void glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
    return g_glFunctions->glVertexAttrib2f(indx, x, y);
}

inline void glVertexAttrib2fv(GLuint indx, const GLfloat* values)
{
    return g_glFunctions->glVertexAttrib2fv(indx, values);
}

inline void glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
    return g_glFunctions->glVertexAttrib3f(indx, x, y, z);
}

inline void glVertexAttrib3fv(GLuint indx, const GLfloat* values)
{
    return g_glFunctions->glVertexAttrib3fv(indx, values);
}

inline void glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    return g_glFunctions->glVertexAttrib4f(indx, x, y, z, w);
}

inline void glVertexAttrib4fv(GLuint indx, const GLfloat* values)
{
    return g_glFunctions->glVertexAttrib4fv(indx, values);
}

inline void glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                  const void* ptr)
{
    return g_glFunctions->glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

}  // namespace CGE4QT_OpenGLFunctions

using namespace CGE4QT_OpenGLFunctions;

#endif

#endif /* _CGEPLATFORMS_H_ */
