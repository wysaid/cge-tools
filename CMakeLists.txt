cmake_minimum_required(VERSION 3.16)

project(cge-tools VERSION 2.0.0 LANGUAGES CXX)

option(BUILD_FILTER_GENERATOR "Build filter generator application" OFF)
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
    find_program(QMAKE_EXECUTABLE NAMES qmake qmake6 HINTS "${Qt_DIR}/bin")
    if (QMAKE_EXECUTABLE)
        execute_process(
                COMMAND "${QMAKE_EXECUTABLE}" -query QT_VERSION
                OUTPUT_VARIABLE QT_VERSION_FULL
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" QT_VERSION ${QT_VERSION_FULL})
        message(STATUS "Found Qt version: ${QT_VERSION}")
    else ()
        set(QT_VERSION 6)
        message(WARNING "qmake not found, assuming Qt 6")
    endif ()
endif ()

if (NOT DEFINED Qt_DIR)
    message(FATAL_ERROR "Qt not found! Please set QTDIR environment variable or CMAKE_PREFIX_PATH")
else ()
    message(STATUS "Using Qt: ${Qt_DIR}, version ${QT_VERSION}")
endif ()

set(CMAKE_PREFIX_PATH ${Qt_DIR} ${CMAKE_PREFIX_PATH})
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
