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

if (NOT DEFINED Qt_DIR OR NOT EXISTS ${Qt_DIR})
    if (DEFINED ENV{QTDIR})
        set(Qt_DIR $ENV{QTDIR})
    elseif (DEFINED ENV{QT_DIR})
        set(Qt_DIR $ENV{QT_DIR})
    else ()
        find_program(MY_QMAKE_NAME qmake qmake6)
        if (MY_QMAKE_NAME)
            get_filename_component(MY_QMAKE_DIR ${MY_QMAKE_NAME} DIRECTORY)
            get_filename_component(Qt_DIR ${MY_QMAKE_DIR} DIRECTORY)
        endif ()
    endif ()
endif ()

# Detect Qt version
if (NOT DEFINED QT_VERSION)
    # Prefer qmake6 (Qt6) over legacy qmake to avoid empty version from Qt5's qmake
    find_program(QMAKE_EXECUTABLE NAMES qmake6 qmake HINTS "${Qt_DIR}/bin")
    if (QMAKE_EXECUTABLE)
        # Also derive Qt_DIR from QMAKE_EXECUTABLE if not yet set (handles cases
        # where find_program above missed qmake but HINTS path succeeded).
        if (NOT DEFINED Qt_DIR OR NOT EXISTS "${Qt_DIR}")
            get_filename_component(_qmake_dir "${QMAKE_EXECUTABLE}" DIRECTORY)
            get_filename_component(Qt_DIR "${_qmake_dir}" DIRECTORY)
        endif ()
        execute_process(
                COMMAND "${QMAKE_EXECUTABLE}" -query QT_VERSION
                OUTPUT_VARIABLE QT_VERSION_FULL
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
        )
        if (QT_VERSION_FULL AND NOT QT_VERSION_FULL STREQUAL "**Unknown**")
            string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" QT_VERSION ${QT_VERSION_FULL})
            message(STATUS "Found Qt version: ${QT_VERSION}")
        else ()
            set(QT_VERSION 6)
            message(WARNING "Could not determine Qt version from qmake output '${QT_VERSION_FULL}', assuming Qt 6")
        endif ()
    else ()
        set(QT_VERSION 6)
        message(WARNING "qmake not found, assuming Qt 6")
    endif ()
endif ()

if (NOT DEFINED Qt_DIR OR NOT EXISTS "${Qt_DIR}")
    # Qt_DIR not found via QTDIR env or qmake — rely on cmake's built-in
    # find_package search paths (works for system-installed Qt on Ubuntu/Debian).
    message(STATUS "Qt dir not detected; relying on system CMake package search paths")
else ()
    message(STATUS "Using Qt: ${Qt_DIR}, version ${QT_VERSION}")
    set(CMAKE_PREFIX_PATH ${Qt_DIR} ${CMAKE_PREFIX_PATH})
endif ()

set(QT Qt${QT_VERSION})

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
    find_package(${QT} REQUIRED COMPONENTS Gui Widgets OpenGLWidgets)
    add_subdirectory(filterGenerator)
else ()
    message(STATUS "Skipping filter generator (use -DBUILD_FILTER_GENERATOR=ON to enable)")
endif ()


message(STATUS "")
message(STATUS "==== CGE Build Summary ====")
message(STATUS "Version:              ${PROJECT_VERSION}")
message(STATUS "Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard:         ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt version:           ${QT_VERSION}")
message(STATUS "Shared libs:          ${BUILD_SHARED_LIBS}")
message(STATUS "Filter generator:     ${BUILD_FILTER_GENERATOR}")
