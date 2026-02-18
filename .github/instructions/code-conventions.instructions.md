---
description: "Code conventions for C++, headers, and Qt integration. Covers naming, OpenGL patterns, and resource management."
applyTo: "src/**/*.cpp,include/**/*.h,include/**/*.hpp,examples/**/*.cpp,filterGenerator/**/*.cpp,filterGenerator/**/*.h"
---

# C++ / Core Library

- Follow `.clang-format` (LLVM-based, Allman braces, 4 spaces, 120 column limit).
- All core code in `namespace CGE {}`.
- C++11 standard only — no C++14/17/20 features.
- Use `#pragma once` for include guards.
- Header includes: system headers first, then Qt, then CGE headers.

## Naming Conventions

- **Classes:** PascalCase with `CGE` prefix: `CGEImageHandler`, `CGEBrightnessFilter`
- **Methods:** camelCase: `setIntensity()`, `processingFilters()`
- **Member variables:** m-prefix: `m_texture`, `m_intensity`
- **Constants:** UPPER_SNAKE_CASE: `MAX_TEXTURE_SIZE`
- **Macros:** `CGE_` prefix: `CGE_SHADER_STRING()`

## OpenGL / Shaders

- OpenGL ES 2.0 compatible code only (no ES 3.0+ features).
- Always check GL errors in Debug builds.
- GLSL via `CGE_SHADER_STRING()` or `CGE_SHADER_STRING_PRECISION_H()` macros.
- Use precision qualifiers in shaders (`mediump`, `highp`).
- Clean up GL resources (textures, FBOs, programs) in destructors.
- Never call OpenGL functions from non-GL threads.

```cpp
// Good
const char* fragShader = CGE_SHADER_STRING_PRECISION_H(
    varying vec2 textureCoordinate;
    uniform sampler2D inputImageTexture;
    
    void main()
    {
        gl_FragColor = texture2D(inputImageTexture, textureCoordinate);
    }
);
```

## Qt Integration

- Check Qt version at compile time:
  ```cpp
  #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
      // Qt6 code
  #else
      // Qt5 code (if legacy support needed)
  #endif
  ```
- Use Qt types for Qt-facing APIs (`QImage`, `QString`).
- Use standard C++ types for internal implementation.
- All Qt widgets must be created on main thread.
- OpenGL contexts must be current before GL calls.

## Error Handling

- Return error codes or bool success/failure (no exceptions).
- Log errors with meaningful messages:
  ```cpp
  CGE_LOG_ERROR("Failed to compile shader: %s\n", errorLog);
  ```
- Validate input parameters (null pointers, invalid sizes).
- Graceful degradation when possible.

## Resource Management

- Use RAII for GL resources (textures, shaders, FBOs).
- Clean up in destructors, not just `cleanup()` methods.
- Check resource limits (max texture size, FBO support).
- Release resources before GL context destruction.

## Filter Implementation

All filters must:
- Inherit from `CGEImageFilterInterface`
- Implement `render2Texture()` with proper viewport setup
- Support intensity adjustment (0.0 to 1.0 typical range)
- Handle both texture input and framebuffer output
- Clean up GL resources in destructor

```cpp
class CGECustomFilter : public CGEImageFilterInterface
{
public:
    void render2Texture(CGEImageHandlerInterface* handler, GLuint srcTexture, GLuint vertexBufferID);
    void setIntensity(float intensity) { m_intensity = intensity; }
    
protected:
    float m_intensity = 1.0f;
};
```

## Memory Management

- Use smart pointers where appropriate (`std::unique_ptr`, `std::shared_ptr`).
- Manual memory management for performance-critical paths (document ownership).
- No memory leaks — verify with Valgrind/AddressSanitizer.
- Be careful with Qt object ownership (parent-child relationships).

## Platform-Specific Code

Use platform checks sparingly:
```cpp
#ifdef Q_OS_MACOS
    // macOS-specific code
#elif defined(Q_OS_WIN)
    // Windows-specific code
#elif defined(Q_OS_LINUX)
    // Linux-specific code
#endif
```

Keep platform-specific code in `cgePlatform_*.cpp` files when possible.

## Documentation

- Document public APIs with comments (Doxygen-style preferred).
- Explain "why" in comments, not "what" (code should be self-explanatory).
- Complex algorithms need high-level explanation.
- Update header documentation when changing behavior.

```cpp
/**
 * Apply brightness adjustment to the image.
 * @param intensity Brightness adjustment factor (0.0 = black, 1.0 = no change, 2.0 = double brightness)
 * @note Values outside [0.0, 2.0] are clamped.
 */
void setIntensity(float intensity);
```

## Testing

- Test filters with various image sizes (square, rect, odd dimensions).
- Test with both Debug and Release builds.
- Verify no GL errors in Debug mode.
- Check for memory leaks with AddressSanitizer.
- Test on all target platforms.

## Performance

- Minimize texture uploads/downloads (keep data on GPU).
- Reuse textures and FBOs when possible.
- Batch operations to reduce state changes.
- Profile before optimizing.
- Document performance-critical sections.

## Deprecation

When deprecating features:
```cpp
[[deprecated("Use newFunction() instead")]]
void oldFunction();
```

Provide migration path in documentation.
