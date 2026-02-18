#!/bin/bash

# CGE Build Script
# Convenient wrapper for CMake build operations

set -e

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$THIS_DIR"
BUILD_DIR="$PROJECT_DIR/build"
BUILD_TYPE="Debug"
JOBS=$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)
BUILD_FILTER_GENERATOR="OFF"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

function print_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  --clean           Clean build directory"
    echo "  --config          Configure CMake project"
    echo "  --build           Build the project"
    echo "  --rebuild         Clean + Config + Build"
    echo "  --release         Use Release build type (default: Debug)"
    echo "  --debug           Use Debug build type (default)"
    echo "  --filter-gen      Enable filter generator build"
    echo "  --install         Install the built libraries"
    echo "  --test            Run tests"
    echo "  --format          Format all source code with clang-format"
    echo "  --xcode           Generate Xcode project (macOS only)"
    echo "  --build-single-file <file>  Build a single cpp file for syntax check"
    echo "  --help            Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 --clean --config --build"
    echo "  $0 --release --rebuild"
    echo "  $0 --filter-gen --build"
    echo "  $0 --xcode"
    echo "  $0 --build-single-file src/core/cgeImageFilter.cpp"
}

function print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

function print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

function print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

function clean_build() {
    print_info "Cleaning build directory: $BUILD_DIR"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_info "Build directory cleaned"
    else
        print_warn "Build directory does not exist"
    fi
}

function configure_cmake() {
    print_info "Configuring CMake project..."
    print_info "Build Type: $BUILD_TYPE"
    print_info "Build Directory: $BUILD_DIR"
    print_info "Filter Generator: $BUILD_FILTER_GENERATOR"

    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    cmake .. \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DBUILD_FILTER_GENERATOR="$BUILD_FILTER_GENERATOR" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

    print_info "CMake configuration complete"
}

function build_project() {
    print_info "Building project..."

    if [ ! -d "$BUILD_DIR" ]; then
        print_warn "Build directory does not exist, configuring first..."
        configure_cmake
    fi

    cd "$BUILD_DIR"
    cmake --build . --config "$BUILD_TYPE" -j "$JOBS"

    print_info "Build complete"

    # Show build output
    if [ "$BUILD_TYPE" = "Debug" ]; then
        LIB_NAME="libcged.a"
    else
        LIB_NAME="libcge.a"
    fi

    if [ -f "$BUILD_DIR/$LIB_NAME" ]; then
        LIB_SIZE=$(ls -lh "$BUILD_DIR/$LIB_NAME" | awk '{print $5}')
        print_info "Built library: $LIB_NAME ($LIB_SIZE)"
    fi
}

function install_project() {
    print_info "Installing project..."

    if [ ! -d "$BUILD_DIR" ]; then
        print_error "Build directory does not exist. Build the project first."
        exit 1
    fi

    cd "$BUILD_DIR"
    sudo cmake --install .

    print_info "Installation complete"
}

function run_tests() {
    print_info "Running tests..."

    if [ ! -d "$BUILD_DIR" ]; then
        print_error "Build directory does not exist. Build the project first."
        exit 1
    fi

    cd "$BUILD_DIR"
    ctest --output-on-failure

    print_info "Tests complete"
}

function format_code() {
    print_info "Formatting source code..."

    if ! command -v clang-format &>/dev/null; then
        print_error "clang-format not found. Please install it first."
        exit 1
    fi

    find "$PROJECT_DIR/include" "$PROJECT_DIR/src" \
        -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) \
        -exec clang-format -i {} \;

    print_info "Code formatting complete"
}

function gen_xcode() {
    print_info "Generating Xcode project..."
    XCODE_BUILD_DIR="$PROJECT_DIR/build_xcode"
    mkdir -p "$XCODE_BUILD_DIR"
    cd "$XCODE_BUILD_DIR"

    cmake .. -G Xcode \
        -DBUILD_FILTER_GENERATOR="$BUILD_FILTER_GENERATOR"

    print_info "Xcode project generated at: $XCODE_BUILD_DIR"
    print_info "Open with: open $XCODE_BUILD_DIR/cge-tools.xcodeproj"
}

function build_single_file() {
    local FILE="$1"
    if [ -z "$FILE" ]; then
        print_error "No file specified for --build-single-file"
        exit 1
    fi

    print_info "Building single file: $FILE"

    if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
        print_warn "compile_commands.json not found, configuring first..."
        configure_cmake
    fi

    # Use clang++ directly with compile_commands.json for syntax checking
    if command -v clang++ &>/dev/null; then
        # Extract compile command from compile_commands.json
        if command -v python3 &>/dev/null && [ -f "$BUILD_DIR/compile_commands.json" ]; then
            ABS_FILE="$(cd "$(dirname "$FILE")" && pwd)/$(basename "$FILE")"
            CMD=$(python3 -c "
import json, sys
data = json.load(open('$BUILD_DIR/compile_commands.json'))
for e in data:
    if e['file'] == '$ABS_FILE' or e['file'].endswith('$(basename "$FILE")'):
        print(e['command'])
        break
")
            if [ -n "$CMD" ]; then
                print_info "Running: $CMD"
                eval "$CMD"
                print_info "Single file build complete"
                return
            fi
        fi
    fi

    # Fallback: build the whole project
    print_warn "Could not find specific compile command, building full project..."
    build_project
}

# Parse command line arguments
if [ $# -eq 0 ]; then
    print_usage
    exit 0
fi

DO_CLEAN=false
DO_CONFIG=false
DO_BUILD=false
DO_INSTALL=false
DO_TEST=false
DO_FORMAT=false
DO_XCODE=false
SINGLE_FILE=""

while [ $# -gt 0 ]; do
    case "$1" in
    --clean)
        DO_CLEAN=true
        ;;
    --config)
        DO_CONFIG=true
        ;;
    --build)
        DO_BUILD=true
        ;;
    --rebuild)
        DO_CLEAN=true
        DO_CONFIG=true
        DO_BUILD=true
        ;;
    --release)
        BUILD_TYPE="Release"
        ;;
    --debug)
        BUILD_TYPE="Debug"
        ;;
    --filter-gen)
        BUILD_FILTER_GENERATOR="ON"
        ;;
    --install)
        DO_INSTALL=true
        ;;
    --test)
        DO_TEST=true
        ;;
    --format)
        DO_FORMAT=true
        ;;
    --xcode)
        DO_XCODE=true
        ;;
    --build-single-file)
        shift
        SINGLE_FILE="$1"
        ;;
    --help | -h)
        print_usage
        exit 0
        ;;
    *)
        print_error "Unknown option: $1"
        print_usage
        exit 1
        ;;
    esac
    shift
done

# Execute tasks in order
[ "$DO_CLEAN" = true ] && clean_build
[ "$DO_CONFIG" = true ] && configure_cmake
[ "$DO_BUILD" = true ] && build_project
[ "$DO_INSTALL" = true ] && install_project
[ "$DO_TEST" = true ] && run_tests
[ "$DO_FORMAT" = true ] && format_code
[ "$DO_XCODE" = true ] && gen_xcode
[ -n "$SINGLE_FILE" ] && build_single_file "$SINGLE_FILE"

print_info "All tasks completed successfully!"
