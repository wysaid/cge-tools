# Contributing Guide

Thank you for your interest in contributing to CGE-Tools! This guide will help you get started.

## Getting Started

1. **Fork** the repository
2. **Clone** your fork: `git clone https://github.com/YOUR_USERNAME/cge-tools.git`
3. **Create** a feature branch: `git checkout -b feat/your-feature` or `fix/bug-description`
4. Make your changes
5. **Test** thoroughly: `./tasks.sh --release --rebuild`
6. **Commit** your changes
7. **Push** to your fork and submit a pull request

## Code Standards

### Commit Messages

Follow conventional commit format:
- `feat:` - New feature
- `fix:` - Bug fix
- `refactor:` - Code refactoring
- `perf:` - Performance improvement
- `docs:` - Documentation only
- `style:` - Code style/formatting
- `test:` - Adding tests
- `chore:` - Build process, dependencies, etc.

Examples:
```
feat: add vignette filter support
fix: correct brightness calculation in CGEBrightnessFilter
docs: update build instructions for Windows
refactor: simplify texture upload path
```

### Code Style

- Use **English** for all code, comments, and documentation
- Follow `.clang-format` configuration (run `./format-all.sh` before committing)
- One logical change per commit
- Detailed style rules in `.github/instructions/code-conventions.instructions.md`

### Code Review

- Self-review your code before submitting
- Respond to review comments promptly
- Keep PR scope focused and manageable
- Update PR description if scope changes

## Compatibility Rules

These are **hard constraints** for the public API:

### ✅ Safe Changes (Always OK)
- Adding new filters (new classes in `include/cge/filters/`)
- Adding new methods to existing classes
- Adding new optional parameters with defaults
- Performance improvements (if behavior unchanged)
- Bug fixes
- Documentation improvements

### ⚠️ Careful Changes (Need Discussion)
- Changing filter behavior (even if "fixing" it)
- Modifying default parameter values
- Renaming public methods (requires deprecation)
- Changing method signatures (requires overload)

### ❌ Breaking Changes (Avoid)
- Removing public methods
- Changing method signatures without backward compatibility
- Removing or renaming classes in `include/cge/`
- Changing expected input/output formats

## Development Workflow

### 1. Build and Test

```bash
# Clean rebuild (recommended after git pull)
./tasks.sh --clean --config --build

# Quick build during development
./tasks.sh --build

# Release build for testing
./tasks.sh --release --rebuild

# Format code
./format-all.sh
```

### 2. Adding a New Filter

1. Create header in `include/cge/filters/cgeYourFilter.h`
2. Create implementation in `src/filters/cgeYourFilter.cpp`
3. Inherit from `CGEImageFilterInterface`
4. Implement required methods (`render2Texture`, etc.)
5. Add example usage to `examples/` if appropriate
6. Update documentation

Example:
```cpp
// include/cge/filters/cgeYourFilter.h
#pragma once
#include "cgeGLFunctions.h"

namespace CGE
{
    class CGEYourFilter : public CGEImageFilterInterface
    {
    public:
        void render2Texture(CGEImageHandlerInterface* handler, 
                          GLuint srcTexture, GLuint vertexBufferID) override;
        void setIntensity(float intensity) { m_intensity = intensity; }
        
    protected:
        float m_intensity = 1.0f;
    };
}
```

### 3. Qt Version Compatibility

The project now supports both Qt5 (5.15+) and Qt6 (6.0+):

- Use version checks for Qt version-specific code:
  ```cpp
  #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
      // Qt6-specific code
  #else
      // Qt5 code (or fallback)
  #endif
  ```
- Test with both Qt5 and Qt6 if possible
- Prefer APIs that work in both versions (e.g., `globalPos()` over `globalPosition()`)
- When APIs differ, use the cross-compatible version
- Key differences:
  - Qt6 has `Qt::OpenGLWidgets` module; Qt5 bundles `QOpenGLWidget` in `Qt::Widgets`
  - Some deprecated APIs removed in Qt6 (e.g., `QGLFunctions`)

### 4. Platform Testing

Ideally test on:
- macOS (primary development platform)
- Linux (Ubuntu/Debian)
- Windows (MSVC)

Minimum requirement: test on your primary platform + verify build on one other.

## Key Constraints

### OpenGL

- **ES 2.0 Compatible**: Code must work on OpenGL ES 2.0 (mobile compatibility)
- **Thread Safety**: All GL calls must be on the GL thread
- **Resource Cleanup**: Always clean up textures, FBOs, shaders in destructors
- **Error Checking**: Check GL errors in Debug builds

### Build System

- **CMake**: When adding source files, they're auto-discovered via `GLOB_RECURSE`
- **Qt Version**: Support Qt 5.15+ and Qt 6.0+, ensure cross-version compatibility
- **C++11**: Use C++11 standard features only (no C++14/17/20)

### filterGenerator

The GUI application now supports both Qt5 and Qt6:
- Enable with `-DBUILD_FILTER_GENERATOR=ON` (enabled by default)
- Qt version is automatically detected at build time
- Don't couple library APIs to filterGenerator-specific features

## Testing Checklist

Before submitting a PR:

- [ ] Code compiles without errors (Debug and Release)
- [ ] Runs `./format-all.sh` to format code
- [ ] No new compiler warnings
- [ ] Tested with example programs
- [ ] Updated documentation if needed
- [ ] Commit messages follow conventional format
- [ ] PR description explains what/why/how

## Pull Request Process

1. **Update your branch** with latest `master`:
   ```bash
   git fetch upstream
   git rebase upstream/master
   ```

2. **Push to your fork**:
   ```bash
   git push origin feat/your-feature
   ```

3. **Create Pull Request**:
   - Clear title summarizing the change
   - Description explaining motivation and implementation
   - Link related issues if any
   - Add screenshots/videos for visual changes

4. **Review Process**:
   - Maintainers will review your code
   - Address feedback by pushing new commits
   - Once approved, PR will be merged

## Getting Help

- **Issues**: Open an issue for bugs or feature requests
- **Discussions**: Use GitHub Discussions for questions
- **Email**: admin@wysaid.org for private matters

## Code of Conduct

- Be respectful and constructive
- Focus on the code, not the person
- Welcome newcomers and help them learn
- Assume good intentions

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (check LICENSE file).

## Recognition

Contributors will be acknowledged in release notes and documentation. Thank you for making CGE-Tools better!
