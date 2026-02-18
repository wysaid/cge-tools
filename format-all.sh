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

# Format include directory
if [ -d "$THIS_DIR/include" ]; then
    print_info "Formatting include directory..."
    find "$THIS_DIR/include" -type f \( -name "*.h" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i
fi

# Format src directory
if [ -d "$THIS_DIR/src" ]; then
    print_info "Formatting src directory..."
    find "$THIS_DIR/src" -type f \( -name "*.cpp" -o -name "*.cc" -o -name "*.h" \) -print0 | xargs -0 clang-format -i
fi

# Format filterGenerator directory (if exists)
if [ -d "$THIS_DIR/filterGenerator" ]; then
    print_info "Formatting filterGenerator directory..."
    find "$THIS_DIR/filterGenerator" -type f \( -name "*.cpp" -o -name "*.h" \) -print0 | xargs -0 clang-format -i
fi

# Format examples directory (if exists)
if [ -d "$THIS_DIR/examples" ]; then
    print_info "Formatting examples directory..."
    find "$THIS_DIR/examples" -type f \( -name "*.cpp" -o -name "*.h" \) -print0 | xargs -0 clang-format -i
fi

print_info "Code formatting complete!"
print_info "All C++ files have been formatted according to .clang-format configuration."
