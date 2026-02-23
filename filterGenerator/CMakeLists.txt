cmake_minimum_required(VERSION 3.16)

if(WIN32)
    project(filterGenerator VERSION 2.0.0 LANGUAGES CXX RC)
else()
    project(filterGenerator VERSION 2.0.0 LANGUAGES CXX)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui ui/widgets)

file(GLOB_RECURSE ALL_SOURCES
    "*.cpp"
    "widgets/*.cpp"
    "widgets/adjustments/*.cpp"
)

# Each executable has its own main — exclude both from shared sources
set(SHARED_SOURCES ${ALL_SOURCES})
list(REMOVE_ITEM SHARED_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/filterRuleTestMain.cpp"
)

file(GLOB_RECURSE GENERATOR_HEADERS
    "*.h"
    "widgets/*.h"
    "widgets/adjustments/*.h"
)

file(GLOB_RECURSE GENERATOR_UI_FILES
    "ui/*.ui"
    "ui/widgets/*.ui"
)

file(GLOB_RECURSE GENERATOR_RESOURCES
    "*.qrc"
)

# ============================================================================
# Helper macro: set up an app target with icons and install rules
# Usage: cge_add_app(<name> <extra_sources...>)
# ============================================================================
macro(cge_add_app TARGET_NAME)
    set(_EXTRA_SOURCES ${ARGN})
    if(APPLE)
        set(MACOSX_BUNDLE_ICON_FILE ${TARGET_NAME}.icns)
        set(_icon_file "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.icns")
        set_source_files_properties(${_icon_file} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        add_executable(${TARGET_NAME} MACOSX_BUNDLE
            ${SHARED_SOURCES} ${_EXTRA_SOURCES}
            ${GENERATOR_HEADERS} ${GENERATOR_UI_FILES} ${GENERATOR_RESOURCES}
            ${_icon_file}
        )
        set_target_properties(${TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER "org.wysaid.${TARGET_NAME}"
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )
    elseif(WIN32)
        add_executable(${TARGET_NAME} WIN32
            ${SHARED_SOURCES} ${_EXTRA_SOURCES}
            ${GENERATOR_HEADERS} ${GENERATOR_UI_FILES} ${GENERATOR_RESOURCES}
            ${TARGET_NAME}.rc
        )
    else()
        add_executable(${TARGET_NAME}
            ${SHARED_SOURCES} ${_EXTRA_SOURCES}
            ${GENERATOR_HEADERS} ${GENERATOR_UI_FILES} ${GENERATOR_RESOURCES}
        )
    endif()

    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/widgets
        ${CMAKE_CURRENT_SOURCE_DIR}/widgets/adjustments
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        cge
        Qt${QT_VERSION}::Core
        Qt${QT_VERSION}::Gui
        Qt${QT_VERSION}::Widgets
        Qt${QT_VERSION}::OpenGL
        Qt${QT_VERSION}::OpenGLWidgets
    )
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_11)

    install(TARGETS ${TARGET_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
    )

    if(UNIX AND NOT APPLE)
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.desktop.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.desktop"
            @ONLY
        )
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.desktop"
            DESTINATION share/applications
        )
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.ico"
            DESTINATION share/pixmaps
            RENAME ${TARGET_NAME}.png
        )
    endif()
endmacro()

# ============================================================================
# Targets
# ============================================================================
cge_add_app(filterGenerator    main.cpp)
cge_add_app(filterRuleTest     filterRuleTestMain.cpp)

# On Windows with MSVC, GLEW must be included before any OpenGL headers.
# Qt's OpenGL headers include gl.h implicitly, so we force-include glew.h first.
# This also covers Qt MOC-generated compilation units where include order
# cannot be controlled through source code changes alone.
if (NOT APPLE)
    # Resolve the GLEW include path — prefer the singular form (set by the
    # Windows CI when passing -DGLEW_INCLUDE_DIR=...), fall back to the plural
    # form provided by CMake's FindGLEW module on Linux.
    if (GLEW_INCLUDE_DIR)
        set(_GLEW_INC "${GLEW_INCLUDE_DIR}")
    elseif (GLEW_INCLUDE_DIRS)
        list(GET GLEW_INCLUDE_DIRS 0 _GLEW_INC)
    endif ()

    if (_GLEW_INC)
        foreach(_tgt filterGenerator filterRuleTest)
            if (MSVC)
                target_compile_options(${_tgt} PRIVATE "/FI${_GLEW_INC}/GL/glew.h")
            else ()
                target_compile_options(${_tgt} PRIVATE "-include" "${_GLEW_INC}/GL/glew.h")
            endif ()
        endforeach()
    endif ()
endif ()

message(STATUS "Filter Generator configured")
message(STATUS "  Sources: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  UI files: ${CMAKE_CURRENT_SOURCE_DIR}/ui")
