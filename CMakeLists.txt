cmake_minimum_required(VERSION 3.16)

project(cge-tools VERSION 2.0.0 LANGUAGES CXX)

option(BUILD_FILTER_GENERATOR "Build filter generator application" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "Using ccache for compilation")
endif ()

include(cmake/findQt.cmake)

find_package(${QT} REQUIRED COMPONENTS OpenGL Core)

if (NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wno-unused-parameter -Wno-reorder)
endif ()

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if (MSVC)
        add_compile_options(
                /utf-8
                /MP
                /W1
                /D_USE_MATH_DEFINES
                /DWIN32_LEAN_AND_MEAN
        )
    endif ()
endif ()

# Fetch and build GLEW from source on non-Apple platforms (Linux + Windows).
# This avoids any system-level GLEW installation requirement.
if (NOT APPLE)
    include(FetchContent)
    set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
            glew
            URL           https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz
            SOURCE_SUBDIR build/cmake
    )
    # GLEW 2.2.0 uses cmake_minimum_required(VERSION 2.8) which CMake 4.0+
    # no longer accepts.  Override so FetchContent can configure it.
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
    FetchContent_MakeAvailable(glew)
    unset(CMAKE_POLICY_VERSION_MINIMUM)
    # GLEW 2.2.0's CMakeLists.txt uses global include_directories() instead of
    # target_include_directories(), so INTERFACE_INCLUDE_DIRECTORIES is not set
    # on the glew_s target.  Patch it here so downstream targets inherit it.
    target_include_directories(glew_s INTERFACE "$<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>")
endif ()

add_subdirectory(cge)

if (BUILD_FILTER_GENERATOR)
    message(STATUS "Building filter generator application")
    if (QT_VERSION_MAJOR GREATER_EQUAL 6)
        find_package(${QT} REQUIRED COMPONENTS Gui Widgets OpenGLWidgets)
    else ()
        # Qt5: QOpenGLWidget lives in Qt5::Widgets; no separate OpenGLWidgets module
        find_package(${QT} REQUIRED COMPONENTS Gui Widgets)
    endif ()
    add_subdirectory(filterGenerator)
else ()
    message(STATUS "Skipping filter generator (use -DBUILD_FILTER_GENERATOR=ON to enable)")
endif ()


message(STATUS "")
message(STATUS "==== CGE Build Summary ====")
message(STATUS "Version:              ${PROJECT_VERSION}")
message(STATUS "Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard:         ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt version:           ${QT_VERSION_MAJOR} (${QT_VERSION})")
message(STATUS "Shared libs:          ${BUILD_SHARED_LIBS}")
message(STATUS "Filter generator:     ${BUILD_FILTER_GENERATOR}")
