# findQt.cmake — Locate Qt 5 or Qt 6 for cge-tools
#
# After inclusion the following variables are set:
#   QT_VERSION_MAJOR  - major version number (5 or 6)
#   QT_VERSION        - full version string (e.g. "6.7.2" or "5.15.2")
#   QT                - target prefix: "Qt5" or "Qt6"  (CACHE INTERNAL)
#   QT_FOUND          - TRUE when Qt was found
#
# CMAKE_PREFIX_PATH is extended with the Qt installation directory so that
# subsequent find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ...) calls work.
#
# Discovery priority (highest first):
#   1. Qt_DIR CMake variable
#   2. QTDIR / QT_DIR / QT_ROOT  environment variables
#   3. Qt6_DIR / Qt5_DIR         environment variables
#   4. CMake built-in package search  (find_package(QT NAMES Qt6 Qt5))
#   5. qmake6 / qmake executable on PATH → derive prefix

if (NOT QT_FOUND)

    # ------------------------------------------------------------------
    # Step 1: collect prefix hints from the caller / environment
    # ------------------------------------------------------------------
    if (DEFINED Qt_DIR AND EXISTS "${Qt_DIR}")
        list(PREPEND CMAKE_PREFIX_PATH "${Qt_DIR}")
    endif ()

    foreach (_env QTDIR QT_DIR QT_ROOT)
        if (DEFINED ENV{${_env}} AND EXISTS "$ENV{${_env}}")
            list(PREPEND CMAKE_PREFIX_PATH "$ENV{${_env}}")
            break()
        endif ()
    endforeach ()

    foreach (_env Qt6_DIR Qt5_DIR)
        if (DEFINED ENV{${_env}} AND EXISTS "$ENV{${_env}}")
            list(PREPEND CMAKE_PREFIX_PATH "$ENV{${_env}}")
            break()
        endif ()
    endforeach ()

    # ------------------------------------------------------------------
    # Step 2: try CMake's normal package search (prefers Qt6 over Qt5)
    # ------------------------------------------------------------------
    find_package(QT NAMES Qt6 Qt5 QUIET)

    # ------------------------------------------------------------------
    # Step 3: fallback — derive prefix from qmake executable
    # ------------------------------------------------------------------
    if (NOT QT_FOUND)
        find_program(QMAKE_EXECUTABLE NAMES qmake6 qmake)
        if (QMAKE_EXECUTABLE)
            get_filename_component(_qmake_bin "${QMAKE_EXECUTABLE}" DIRECTORY)
            get_filename_component(_qt_prefix "${_qmake_bin}"        DIRECTORY)
            message(STATUS "Qt not found via CMake search; retrying with qmake path: ${_qt_prefix}")
            list(PREPEND CMAKE_PREFIX_PATH "${_qt_prefix}")
            find_package(QT NAMES Qt6 Qt5 QUIET)
        endif ()
    endif ()

    # ------------------------------------------------------------------
    # Bail out if still not found
    # ------------------------------------------------------------------
    if (NOT QT_FOUND)
        message(FATAL_ERROR
            "Qt (5 or 6) not found.\n"
            "Please set one of the following:\n"
            "  - CMake variable:  -DQt_DIR=<Qt install prefix>\n"
            "  - Env variable:    QTDIR, QT_DIR, or QT_ROOT\n"
            "  - Ensure qmake or qmake6 is on PATH")
    endif ()

    # ------------------------------------------------------------------
    # Expose stable variables for the rest of the build
    # ------------------------------------------------------------------
    set(QT "Qt${QT_VERSION_MAJOR}" CACHE INTERNAL "Qt target prefix (Qt5 or Qt6)")
    message(STATUS "Using ${QT}: ${QT_DIR} (version ${QT_VERSION})")

endif () # NOT QT_FOUND
