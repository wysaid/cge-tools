cmake_minimum_required(VERSION 3.16)

# ============================================================================
# Collect all source files
# ============================================================================

# Core sources
file(GLOB_RECURSE CGE_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp
)

# Platform sources
file(GLOB_RECURSE CGE_PLATFORM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/*.cpp
)

# Animation sources
file(GLOB_RECURSE CGE_ANIMATION_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/animation/*.cpp
)

# Sprite sources
file(GLOB_RECURSE CGE_SPRITE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sprite/*.cpp
)

# Slideshow sources
file(GLOB_RECURSE CGE_SLIDESHOW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/slideshow/*.cpp
)

# Filter sources (all subdirectories)
file(GLOB_RECURSE CGE_FILTER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filters/*.cpp
)

# Extend sources (optional modules)
file(GLOB_RECURSE CGE_EXTEND_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/extend/*.cpp
)

if(NOT BUILD_VIDEO_EXTENSIONS)
    list(FILTER CGE_EXTEND_SOURCES EXCLUDE REGEX "/video/")
endif()

# Combine all sources
set(CGE_SOURCES
    ${CGE_CORE_SOURCES}
    ${CGE_PLATFORM_SOURCES}
    ${CGE_ANIMATION_SOURCES}
    ${CGE_SPRITE_SOURCES}
    ${CGE_SLIDESHOW_SOURCES}
    ${CGE_FILTER_SOURCES}
    ${CGE_EXTEND_SOURCES}
)

# ============================================================================
# Create library
# ============================================================================
add_library(cge ${CGE_SOURCES})

# ============================================================================
# Compiler definitions
# ============================================================================
target_compile_definitions(cge
    PUBLIC
        LIBCGE4QT_LIB
    PRIVATE
        CGE_TEXTURE_PREMULTIPLIED=1
)

# Set output name based on build type
set_target_properties(cge PROPERTIES
    OUTPUT_NAME "$<IF:$<CONFIG:Debug>,cged,cge>"
    DEBUG_POSTFIX ""
    VERSION ${PROJECT_VERSION}
    SOVERSION 2
)

# ============================================================================
# Include directories
# ============================================================================
target_include_directories(cge
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        # Allow internal files to include from src directories
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/filters
        ${CMAKE_CURRENT_SOURCE_DIR}/src/filters/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/src/slideshow
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extend
)

# ============================================================================
# Link libraries
# ============================================================================
target_link_libraries(cge
    PUBLIC
        ${QT}::Core
        ${QT}::OpenGL
)

#  ============================================================================
# Platform-specific linking
# ============================================================================
if(APPLE)
    target_link_libraries(cge PUBLIC "-framework OpenGL")
    if(NOT IOS)
        target_link_libraries(cge PUBLIC "-framework AppKit")
    endif()
elseif(WIN32)
    target_link_libraries(cge PUBLIC opengl32)
elseif(UNIX)
    target_link_libraries(cge PUBLIC GL)
endif()

# ============================================================================
# Installation
# ============================================================================
include(GNUInstallDirs)

install(TARGETS cge
    EXPORT cgeTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install public headers
install(DIRECTORY include/cge
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT cgeTargets
    FILE cgeTargets.cmake
    NAMESPACE CGE::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cge
)

# Create package config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cgeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cgeConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cge
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cgeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cgeConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cgeConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cge
)

# ============================================================================
# Summary
# ============================================================================
message(STATUS "CGE Library configuration:")
message(STATUS "  Core sources:      ${CMAKE_CURRENT_LIST_DIR}/src/core")
message(STATUS "  Filter sources:    ${CMAKE_CURRENT_LIST_DIR}/src/filters")
message(STATUS "  Public headers:    ${CMAKE_CURRENT_LIST_DIR}/include/cge")
message(STATUS "  Total sources:     ${CGE_SOURCES}" | wc -w | xargs echo "files")
