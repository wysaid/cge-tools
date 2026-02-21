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
#include <cstdio>
#include <cstring>

// On non-macOS desktop platforms, use GLEW for OpenGL function loading.
// GLEW must be included BEFORE any other OpenGL headers.
#if !defined(QT_OPENGL_ES_2) && !defined(Q_OS_MAC)
#include <GL/glew.h>
#define CGE_USE_GLEW 1
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
// QOpenGLFunctions and QOpenGLContext headers call #undef on GLEW-provided OpenGL
// function pointers when included after glew.h. On desktop Linux/Windows where GLEW
// is used for function loading, skip these Qt OpenGL headers entirely — GLEW alone
// provides all the raw glXxx() symbols the library needs.
#ifndef CGE_USE_GLEW
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#endif
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

#endif /* _CGEPLATFORMS_H_ */
