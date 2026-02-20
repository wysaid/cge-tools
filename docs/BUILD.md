# Building CGE

This document provides detailed instructions for building the CGE library and tools.

## Quick Start

```bash
# macOS/Linux
export QTDIR=/path/to/Qt/6.x.x/macos  # or linux
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)

# Windows
set QTDIR=C:\Qt\6.x.x\msvc2019_64
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Build Options

### Standard Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_SHARED_LIBS` | `OFF` | Build shared libraries instead of static |
| `BUILD_FILTER_GENERATOR` | `ON` | Build the filter generator GUI application |
| `CMAKE_BUILD_TYPE` | - | Build type: `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel` |
| `CMAKE_PREFIX_PATH` | `$ENV{QTDIR}` | Path to Qt installation |

### Examples

```bash
# Build shared library
cmake .. -DBUILD_SHARED_LIBS=ON

# Exclude filter generator
cmake .. -DBUILD_FILTER_GENERATOR=OFF

# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Custom Qt path
cmake .. -DCMAKE_PREFIX_PATH=/custom/path/to/Qt/6.7.1/macos

# Combine options
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=/usr/local
```

## Build Outputs

### Libraries

| Build Type | Output |
|------------|--------|
| Release (static) | `build/libcge.a` (Unix) / `build/cge.lib` (Windows) |
| Debug (static) | `build/libcge.a` (Unix) / `build/cged.lib` (Windows/MSVC) |
| Release (shared) | `build/libcge.so` (Linux) / `build/libcge.dylib` (macOS) / `build/cge.dll` (Windows) |
| Debug (shared) | `build/libcge.so` (Linux) / `build/libcge.dylib` (macOS) / `build/cged.dll` (Windows/MSVC) |

### Applications

| Application | Output | Status |
|-------------|--------|--------|
| filterGenerator | `build/filterGenerator/filterGenerator.app` (macOS)<br>`build/filterGenerator/filterGenerator` (Linux)<br>`build/filterGenerator/filterGenerator.exe` (Windows) | Functional (built by default) |

## Installation

Install the library and headers system-wide:

```bash
cd build
cmake --install . --prefix /usr/local
```

This installs:
- Headers to `${prefix}/include/cge/`
- Library to `${prefix}/lib/`
- CMake config files to `${prefix}/lib/cmake/cge/`

## Using CGE in Your Project

### With CMake (Recommended)

After installation:

```cmake
find_package(cge REQUIRED)
target_link_libraries(your_app PRIVATE CGE::cge)
```

Without installation (using build directory):

```cmake
set(cge_DIR /path/to/cge-tools/build)
find_package(cge REQUIRED)
target_link_libraries(your_app PRIVATE CGE::cge)
```

### Manual Integration

```cmake
target_include_directories(your_app PRIVATE /path/to/cge-tools/include)
target_link_libraries(your_app PRIVATE 
    /path/to/cge-tools/build/libcge.a
    Qt6::Core 
    Qt6::OpenGL
)
```

## Platform-Specific Notes

### macOS

- Requires Xcode Command Line Tools
- Qt6 can be installed via Homebrew: `brew install qt@6`
- Set `QTDIR` to Qt installation: `export QTDIR=$(brew --prefix qt@6)`

### Linux

#### Ubuntu/Debian

```bash
sudo apt-get install build-essential cmake qt6-base-dev qt6-opengl-dev
```

#### Fedora/RHEL

```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel
```

### Windows

- Requires Visual Studio 2019 or later
- Install Qt6 from [qt.io](https://www.qt.io/download)
- Use Qt's provided CMake or Visual Studio integration

#### Visual Studio

Open Visual Studio Developer Command Prompt:

```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64 ^
  -DCMAKE_PREFIX_PATH=C:\Qt\6.7.1\msvc2019_64
cmake --build . --config Release
```

## Troubleshooting

### Qt6 Not Found

```
CMake Error: Could not find a package configuration file provided by "Qt6"
```

**Solution**: Set `CMAKE_PREFIX_PATH` or `QTDIR`:

```bash
export QTDIR=/path/to/Qt/6.x.x/macos
# or
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/macos
```

### OpenGL Headers Not Found (Linux)

```bash
sudo apt-get install mesa-common-dev libgl1-mesa-dev
```

### filterGenerator Build Errors

filterGenerator is built by default. If you encounter build errors, ensure Qt6 is properly installed. To disable:

```bash
cmake .. -DBUILD_FILTER_GENERATOR=OFF
```

## Clean Build

```bash
# Remove build directory
rm -rf build

# Or clean within build directory
cd build
cmake --build . --target clean
```

## Development

### Running Tests

```bash
cd build
ctest
```

### Generating Documentation

```bash
# Install Doxygen
cd build
cmake .. -DBUILD_DOCUMENTATION=ON
cmake --build . --target docs
```

## Cross-Compilation

### Android

```bash
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-21
```

### iOS

```bash
cmake .. \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
  -DCMAKE_OSX_ARCHITECTURES=arm64
```

## Performance

### Optimization Flags

```bash
# Maximum optimization
cmake .. -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native"

# Debug with optimizations
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

### Static vs Shared Libraries

- **Static** (`BUILD_SHARED_LIBS=OFF`): Better performance, larger binary size
- **Shared** (`BUILD_SHARED_LIBS=ON`): Smaller binary size, may require runtime paths

## Getting Help

- GitHub Issues: https://github.com/wysaid/cge-tools/issues
- Documentation: See README.md
