# CGE Examples

This directory contains sample programs showing how to use the CGE library.

## Prerequisites

1. CGE static library has been built (`libcge.a` or `libcged.a`)
2. Qt6 is installed (6.0 or later)

## Build the examples

```bash
# First, make sure CGE has been built
cd ..
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j8
cd ..

# Then build the examples
cd examples
mkdir -p build && cd build
cmake ..
cmake --build .
```

## simple_filter - basic filter example

This example shows how to:
- load an image
- create an image handler
- apply a brightness filter
- save the processed image

### Run the example

```bash
# Prepare a test image
cp /path/to/your/image.jpg input.jpg

# Run the example
./simple_filter

# View the output
open output.jpg  # macOS
# or
xdg-open output.jpg  # Linux
```

## Available filters

CGE provides 70+ image filters, including:

- **Basic adjustments**: brightness, contrast, exposure, hue, saturation
- **Color**: level, curve, color balance
- **Blur**: gaussian blur, bilateral blur, motion blur
- **Effects**: edge detection, emboss, mosaic, liquify
- **Beautify**: skin smoothing, face slimming, eye enlargement
- **Blend**: multiple blend modes

For the full list, see [include/cge/filters](../include/cge/filters).

## Use different filters

Change the filter type in `simple_filter.cpp`:

```cpp
// Brightness
CGEBrightnessFilter* filter = new CGEBrightnessFilter();
filter->setIntensity(0.5f);

// Contrast
CGEContrastFilter* filter = new CGEContrastFilter();
filter->setIntensity(0.3f);

// Gaussian blur
CGEGaussianBlurFilter* filter = new CGEGaussianBlurFilter();
filter->setBlurLevel(5.0f);
```

## Use CGE in your project

### CMakeLists.txt

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL)

add_executable(your_app main.cpp)
target_link_libraries(your_app 
    PRIVATE
    cge
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
)
```

### Code example

```cpp
#include "cge.h"

// 1. Create handler
CGE::CGEImageHandler handler;
handler.initWithQImage(&image);

// 2. Add filter
CGE::CGEBrightnessFilter* filter = new CGE::CGEBrightnessFilter();
filter->setIntensity(0.5f);
handler.addFilter(filter);

// 3. Process image
handler.processingFilters();

// 4. Get result
QImage result = handler.getResultImage();
```

## Troubleshooting

### Cannot find CGE library

Make sure `CGE_DIR` points to the correct CGE source directory, or ensure CGE has been installed and `CMAKE_PREFIX_PATH` is configured.

### Qt version issue

CGE requires Qt6. If both Qt5 and Qt6 are installed, set `CMAKE_PREFIX_PATH` to Qt6:

```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.7.1/macos ..
```

## More information

- [CGE main documentation](../README.md)
- [Build guide](../docs/BUILD.md)
- [Quick start](../docs/QUICKSTART.md)
- [Online docs](https://github.com/wysaid/cge-tools) (to be improved)
