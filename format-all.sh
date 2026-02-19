#!/bin/bash

# Format all C++ source files with clang-format

set -e

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

function print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

function print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

# Check if clang-format is available
if ! command -v clang-format &>/dev/null; then
    print_warn "clang-format not found. Installing via homebrew..."
    if command -v brew &>/dev/null; then
        brew install clang-format
    else
        echo "Please install clang-format manually."
        exit 1
    fi
fi

print_info "Formatting all C++ source files..."

# Collect directories to format
FORMAT_DIRS=()

# Core library: headers and sources live under cge/
if [ -d "$THIS_DIR/cge" ]; then
    FORMAT_DIRS+=("$THIS_DIR/cge")
fi

# filterGenerator (optional Qt6 GUI app)
if [ -d "$THIS_DIR/filterGenerator" ]; then
    FORMAT_DIRS+=("$THIS_DIR/filterGenerator")
fi

# examples
if [ -d "$THIS_DIR/examples" ]; then
    FORMAT_DIRS+=("$THIS_DIR/examples")
fi

if [ ${#FORMAT_DIRS[@]} -eq 0 ]; then
    print_warn "No source directories found to format."
    exit 0
fi

print_info "Directories: ${FORMAT_DIRS[*]}"

find "${FORMAT_DIRS[@]}" -type f \( -name "*.cpp" -o -name "*.h" \) -print0 | xargs -0 clang-format -i

print_info "Code formatting complete!"
print_info "All C++ files have been formatted according to .clang-format configuration."
