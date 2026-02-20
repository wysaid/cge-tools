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

file(GLOB_RECURSE GENERATOR_SOURCES
    "*.cpp"
    "widgets/*.cpp"
    "widgets/adjustments/*.cpp"
)

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

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE filterGenerator.icns)
    set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/filterGenerator.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(filterGenerator MACOSX_BUNDLE
        ${GENERATOR_SOURCES}
        ${GENERATOR_HEADERS}
        ${GENERATOR_UI_FILES}
        ${GENERATOR_RESOURCES}
        ${app_icon_macos}
    )
elseif(WIN32)
    add_executable(filterGenerator WIN32
        ${GENERATOR_SOURCES}
        ${GENERATOR_HEADERS}
        ${GENERATOR_UI_FILES}
        ${GENERATOR_RESOURCES}
        filterGenerator.rc
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
# cge's PUBLIC include dirs are inherited transitively via target_link_libraries
target_include_directories(filterGenerator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/adjustments
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

# ============================================================================
# Linux: install .desktop file and icon for system integration
# ============================================================================
if(UNIX AND NOT APPLE)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/filterGenerator.desktop.in"
        "${CMAKE_CURRENT_BINARY_DIR}/filterGenerator.desktop"
        @ONLY
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/filterGenerator.desktop"
        DESTINATION share/applications
    )
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/filterGenerator.ico"
        DESTINATION share/pixmaps
        RENAME filterGenerator.png
    )
endif()

message(STATUS "Filter Generator configured")
message(STATUS "  Sources: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  UI files: ${CMAKE_CURRENT_SOURCE_DIR}/ui")
