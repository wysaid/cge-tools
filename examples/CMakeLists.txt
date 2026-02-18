cmake_minimum_required(VERSION 3.16)

project(CGEExamples LANGUAGES CXX)

# ============================================================================
# Simple filter example
# ============================================================================
add_executable(simple_filter simple_filter.cpp)

target_include_directories(simple_filter PRIVATE
    ${CMAKE_SOURCE_DIR}/cge/include
    ${CMAKE_SOURCE_DIR}/cge/src
)

target_link_libraries(simple_filter PRIVATE
    cge
    Qt${QT_VERSION}::Core
    Qt${QT_VERSION}::Gui
    Qt${QT_VERSION}::OpenGL
)

if(QT_VERSION EQUAL 6)
    find_package(Qt6 COMPONENTS OpenGLWidgets)
    if(Qt6OpenGLWidgets_FOUND)
        target_link_libraries(simple_filter PRIVATE Qt6::OpenGLWidgets)
    endif()
endif()

target_compile_features(simple_filter PRIVATE cxx_std_11)

message(STATUS "Examples configured")
message(STATUS "  simple_filter: ${CMAKE_CURRENT_SOURCE_DIR}/simple_filter.cpp")
