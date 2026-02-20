/*
 * platform.h
 *
 * Platform abstraction layer
 */

#pragma once

// Include appropriate platform header based on defines
#if defined(__APPLE__)
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#error "iOS is not supported in this release."
#else
// macOS - use Qt platform
#include "cgePlatform_QT.h"
#endif
#elif defined(__ANDROID__)
#error "Android is not supported in this release."
#elif defined(_WIN32) || defined(_WIN64) || defined(__linux__)
// Windows and Linux - check if Qt is available
#ifdef QT_VERSION
#include "cgePlatform_QT.h"
#else
#include "cgePlatform_GLEW.h"
#endif
#else
#error "Unsupported platform"
#endif
