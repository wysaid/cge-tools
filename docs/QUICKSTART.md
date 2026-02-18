# CGE Quick Start Guide

## Get Started in 5 Minutes

### 1. Install Dependencies

**macOS:**
```bash
# Install Qt6 (if not already installed)
brew install qt@6
export QTDIR=$(brew --prefix qt@6)
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake qt6-base-dev qt6-opengl-dev
```

**Windows:**
- Download and install Qt6 from [Qt official website](https://www.qt.io/download)
- Install Visual Studio 2019 or later

### 2. Clone and Build

```bash
# Clone the repository
git clone https://github.com/wysaid/cge-tools.git
cd cge-tools

# Build the library
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

After building, you will have `libcge.a` (or `libcge.lib` on Windows).

### 3. Run Your First Example

```bash
# Go to examples directory
cd ../examples
mkdir build && cd build

# Build examples
cmake ..
cmake --build .

# Prepare a test image
cp /path/to/your/image.jpg input.jpg

# Run example
./simple_filter

# View result
open output.jpg  # macOS
# or xdg-open output.jpg  # Linux
# or start output.jpg  # Windows
```

## Common Operations

### Apply a Single Filter

```cpp
#include "cge.h"
#include <QImage>

using namespace CGE;

int main() {
    // Load image
    QImage image("input.jpg");

    // Create handler
    CGEImageHandler handler;
    handler.initWithQImage(&image);

    // Add filter
    auto* filter = new CGEBrightnessFilter();
    filter->setIntensity(0.5f);  // Increase brightness by 50%
    handler.addFilter(filter);

    // Process
    handler.processingFilters();

    // Save result
    handler.getResultImage().save("output.jpg");

    return 0;
}
```

### Apply Multiple Filters

```cpp
// Create a filter chain
auto* brightness = new CGEBrightnessFilter();
brightness->setIntensity(0.3f);
handler.addFilter(brightness);

auto* contrast = new CGEContrastFilter();
contrast->setIntensity(0.2f);
handler.addFilter(contrast);

auto* saturation = new CGESaturationFilter();
saturation->setIntensity(0.5f);
handler.addFilter(saturation);

// Process all filters at once
handler.processingFilters();
```

### Real-time Preview (Qt Widgets)

```cpp
class ImageWidget : public QOpenGLWidget {
    CGEImageHandler m_handler;

protected:
    void initializeGL() override {
        m_handler.initWithQImage(&m_image);
    }

    void paintGL() override {
        m_handler.drawResult();
    }

public:
    void applyFilter(CGEImageFilterInterface* filter) {
        m_handler.setFilterWithConfig(filter);
        update();  // Trigger redraw
    }
};
```

## Filter Quick Reference

### Basic Adjustments
```cpp
// Brightness (-1.0 to 1.0)
CGEBrightnessFilter* brightness = new CGEBrightnessFilter();
brightness->setIntensity(0.5f);

// Contrast (-1.0 to 1.0)
CGEContrastFilter* contrast = new CGEContrastFilter();
contrast->setIntensity(0.3f);

// Saturation (0.0 to 2.0)
CGESaturationFilter* saturation = new CGESaturationFilter();
saturation->setIntensity(1.5f);

// Exposure (-10.0 to 10.0)
CGEExposureFilter* exposure = new CGEExposureFilter();
exposure->setIntensity(1.0f);
```

### Blur Effects
```cpp
// Gaussian blur
CGEGaussianBlurFilter* blur = new CGEGaussianBlurFilter();
blur->setBlurLevel(5.0f);

// Bilateral blur (edge-preserving)
CGEBilateralBlurFilter* bilateral = new CGEBilateralBlurFilter();
bilateral->setDistanceNormalizationFactor(3.0f);

// Mosaic
CGEMosaicBlurFilter* mosaic = new CGEMosaicBlurFilter();
mosaic->setIntensity(0.05f);
```

### Beauty Filters
```cpp
// Skin smoothing
CGEBeautifyFilter* beautify = new CGEBeautifyFilter();
beautify->setIntensity(0.7f);

// Eye enlargement
CGEEnlargeEyeFilter* eyeEnlarge = new CGEEnlargeEyeFilter();
eyeEnlarge->setIntensity(0.5f);

// Eye brightening
CGEEyeBrightenFilter* eyeBright = new CGEEyeBrightenFilter();
eyeBright->setIntensity(0.3f);
```

### Artistic Effects
```cpp
// Sketch effect
CGEEdgeDetectFilter* edge = new CGEEdgeDetectFilter();

// Emboss effect
CGEEmbossFilter* emboss = new CGEEmbossFilter();
emboss->setIntensity(1.0f);

// Halftone effect
CGEHalftoneFilter* halftone = new CGEHalftoneFilter();
halftone->setDotSize(0.01f);
```

### LUT Filter (Color Mapping)
```cpp
CGELookupFilter* lut = new CGELookupFilter();
lut->setLookupImagePath("lut.png");  // Load LUT image
lut->setIntensity(1.0f);
```

## Project Integration

### CMakeLists.txt Configuration

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL Widgets OpenGLWidgets)

# Add CGE library path
set(CGE_DIR "/path/to/cge-tools")
set(CGE_LIB "${CGE_DIR}/build/libcge.a")
set(CGE_INCLUDE "${CGE_DIR}/include")

# Your executable
add_executable(myapp main.cpp)

target_include_directories(myapp PRIVATE ${CGE_INCLUDE})
target_link_libraries(myapp
    PRIVATE
    ${CGE_LIB}
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# macOS-specific configuration
if(APPLE)
    target_link_libraries(myapp PRIVATE "-framework OpenGL")
endif()
```

### Manual Compilation (Without CMake)

```bash
# Compile your program
g++ -std=c++11 \
    -I/path/to/cge-tools/include \
    -I$QTDIR/include \
    -I$QTDIR/include/QtCore \
    -I$QTDIR/include/QtGui \
    -I$QTDIR/include/QtOpenGL \
    main.cpp \
    -L/path/to/cge-tools/build \
    -L$QTDIR/lib \
    -lcge \
    -lQt6Core \
    -lQt6Gui \
    -lQt6OpenGL \
    -framework OpenGL \
    -o myapp
```

## FAQ

### Q: Qt not found at compile time

**A:** Set the QTDIR environment variable or use CMAKE_PREFIX_PATH:
```bash
export QTDIR=/path/to/Qt/6.7.1/macos
# or
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.7.1/macos ..
```

### Q: Undefined symbols at link time

**A:** Ensure all required Qt modules are linked:
```cmake
target_link_libraries(myapp
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Widgets      # If using Widgets
    Qt6::OpenGLWidgets  # If using OpenGL Widgets
)
```

### Q: Lots of OpenGL deprecation warnings on macOS

**A:** This is expected. Apple deprecated OpenGL after macOS 10.14, but it still works. To suppress warnings:
```bash
cmake .. -DCMAKE_CXX_FLAGS="-Wno-deprecated-declarations"
```

### Q: filterGenerator app fails to compile

**A:** filterGenerator is undergoing Qt6 migration and is disabled by default. To use it, either wait for full Qt6 support or use the precompiled version in the `tools/` directory.

## Performance Tips

1. **Use Release build**: `cmake .. -DCMAKE_BUILD_TYPE=Release`
2. **Batch processing**: Add multiple filters at once and call `processingFilters()` once
3. **GPU acceleration**: Ensure hardware OpenGL is used, not software rendering
4. **Image size**: Large images process slowly; consider scaling first

## Next Steps

- Read the [Full Documentation](README.md)
- Browse the [Example Programs](examples/)
- Read the [Build Instructions](BUILD.md)
- Explore the [Filter Source Code](src/filters/)

## Help & Support

- **Issue Tracker**: [GitHub Issues](https://github.com/wysaid/cge-tools/issues)
- **Email**: admin@wysaid.org
- **Related Projects**:
  - [Android Version](https://github.com/wysaid/android-gpuimage-plus)
  - [iOS Version](https://github.com/wysaid/ios-gpuimage-plus)

---

Happy Coding! 🎨
