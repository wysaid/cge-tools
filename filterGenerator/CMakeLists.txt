cmake_minimum_required(VERSION 3.16)

project(filterGenerator VERSION 2.0.0 LANGUAGES CXX)

# ============================================================================
# Qt Settings
# ============================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui ui/widgets)

# ============================================================================
# Sources
# ============================================================================
file(GLOB_RECURSE GENERATOR_SOURCES
    "*.cpp"
    "widgets/*.cpp"
    "widgets/adjustments/*.cpp"
)

# Exclude filterRuleTestMain.cpp which has its own main() function
list(REMOVE_ITEM GENERATOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/filterRuleTestMain.cpp")

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
# Executable
# ============================================================================
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    # set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/icon.icns")
    # set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    
    add_executable(filterGenerator MACOSX_BUNDLE
        ${GENERATOR_SOURCES}
        ${GENERATOR_HEADERS}
        ${GENERATOR_UI_FILES}
        ${GENERATOR_RESOURCES}
        # ${app_icon_macos}
    )
else()
    add_executable(filterGenerator
        ${GENERATOR_SOURCES}
        ${GENERATOR_HEADERS}
        ${GENERATOR_UI_FILES}
        ${GENERATOR_RESOURCES}
    )
endif()

# ============================================================================
# Include Directories
# ============================================================================
target_include_directories(filterGenerator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/adjustments
    ${CMAKE_SOURCE_DIR}/cge/include
    ${CMAKE_SOURCE_DIR}/cge/include/cge
    ${CMAKE_SOURCE_DIR}/cge/src
    ${CMAKE_SOURCE_DIR}/cge/src/core
    ${CMAKE_SOURCE_DIR}/cge/src/filters
    ${CMAKE_SOURCE_DIR}/cge/src/filters/adjustments
    ${CMAKE_SOURCE_DIR}/cge/src/filters/internal
    ${CMAKE_SOURCE_DIR}/cge/src/filters/blur
    ${CMAKE_SOURCE_DIR}/cge/src/filters/effects
    ${CMAKE_SOURCE_DIR}/cge/src/filters/beauty
    ${CMAKE_SOURCE_DIR}/cge/src/filters/sharpen
    ${CMAKE_SOURCE_DIR}/cge/src/animation
    ${CMAKE_SOURCE_DIR}/cge/src/sprite
    ${CMAKE_SOURCE_DIR}/cge/src/extend
    ${CMAKE_SOURCE_DIR}/cge/src/platform
)

# ============================================================================
# Link Libraries
# ============================================================================
target_link_libraries(filterGenerator PRIVATE
    cge
    Qt${QT_VERSION}::Core
    Qt${QT_VERSION}::Gui
    Qt${QT_VERSION}::Widgets
    Qt${QT_VERSION}::OpenGL
    Qt${QT_VERSION}::OpenGLWidgets
)

# ============================================================================
# Compiler Features
# ============================================================================
target_compile_features(filterGenerator PRIVATE cxx_std_11)

# ============================================================================
# Installation (optional)
# ============================================================================
if(APPLE)
    set_target_properties(filterGenerator PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.wysaid.filterGenerator"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

install(TARGETS filterGenerator
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

message(STATUS "Filter Generator configured")
message(STATUS "  Sources: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  UI files: ${CMAKE_CURRENT_SOURCE_DIR}/ui")
