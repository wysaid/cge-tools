cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE CGE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(cge ${CGE_SOURCES})

target_compile_definitions(cge PUBLIC LIBCGE4QT_LIB
        PRIVATE CGE_TEXTURE_PREMULTIPLIED=1
)

if (MSVC)
    set_target_properties(cge PROPERTIES
            OUTPUT_NAME cge
            DEBUG_POSTFIX "d"
            VERSION ${PROJECT_VERSION}
            SOVERSION 2
    )
else ()
    set_target_properties(cge PROPERTIES
            OUTPUT_NAME cge
            VERSION ${PROJECT_VERSION}
            SOVERSION 2
    )
endif ()


target_include_directories(cge PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/filters
        ${CMAKE_CURRENT_SOURCE_DIR}/platform
        ${CMAKE_CURRENT_SOURCE_DIR}/sprite
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

target_link_libraries(cge PUBLIC
        ${QT}::Core
        ${QT}::OpenGL
)

# Use GLEW for OpenGL function loading on non-Apple desktop platforms.
if (NOT APPLE)
    find_package(GLEW REQUIRED)
    target_link_libraries(cge PUBLIC GLEW::GLEW)
endif ()

if (APPLE)
    target_link_libraries(cge PUBLIC "-framework OpenGL")
    if (NOT IOS)
        target_link_libraries(cge PUBLIC "-framework AppKit")
    endif ()
elseif (WIN32)
    target_link_libraries(cge PUBLIC opengl32)
elseif (UNIX)
    target_link_libraries(cge PUBLIC GL)
endif ()

