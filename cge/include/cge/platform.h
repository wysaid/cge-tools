/*
 * platform.h
 *
 * Platform abstraction layer
 */

#ifndef _CGE_PLATFORM_H_
#define _CGE_PLATFORM_H_

// Include appropriate platform header based on defines
#if defined(__APPLE__)
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #include "../../src/platform/cgePlatform_IOS.h"
    #else
        // macOS - use Qt platform
        #include "../../src/platform/cgePlatform_QT.h"
    #endif
#elif defined(__ANDROID__)
    #include "../../src/platform/cgePlatform_ANDROID.h"
#elif defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    // Windows and Linux - check if Qt is available
    #ifdef QT_VERSION
        #include "../../src/platform/cgePlatform_QT.h"
    #else
        #include "../../src/platform/cgePlatform_GLEW.h"
    #endif
#else
    #error "Unsupported platform"
#endif

#endif // _CGE_PLATFORM_H_
