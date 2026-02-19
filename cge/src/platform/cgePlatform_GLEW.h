/*
 * cgePlatforms.h
 *
 *  Created on: 2013-12-31
 *      Author: Wang Yang
 *  Description: load some library and do some essential initialization before compiling.
 */

#ifndef _CGEPLATFORM_GLEW_H_
#define _CGEPLATFORM_GLEW_H_

#include <cstdio>

#if defined(DEBUG) || defined(_DEBUG) || defined(CGE_KEEP_LOG_INFO) || defined(CGE_KEEP_LOG_ERROR)

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

#if defined(_CGE_GENERAL_ERROR_TEST_) && _CGE_GENERAL_ERROR_TEST_
#define CGE_UNEXPECTED_ERR_MSG(...) __VA_ARGS__
#else
#define CGE_UNEXPECTED_ERR_MSG(...)
#endif

////////// Preprocess GLEW-related content //////////////////////////////////////////////

#include "glew/glew.h"
#include "gl/GL.h"

#endif /* _CGEPLATFORM_GLEW_H_ */
