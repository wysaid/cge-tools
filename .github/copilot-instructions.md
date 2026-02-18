# GitHub Copilot Instructions — cge-tools

C++ OpenGL image/video processing library with Qt6 integration.

## Stability Rules

- **Never break public API**: Header files in `include/cge/` are the stable contract.
- **Qt6 compatibility**: All Qt code must work with Qt 6.0+.
- **OpenGL**: Support both standard OpenGL and OpenGL ES.
- Prefer **additive** changes over refactoring existing well-tested filters.

## Project Layout

| Path | Purpose |
|------|---------|
| `include/cge/` | Public headers (core, filters, animation, sprite, slideshow, extend) |
| `src/` | Implementation files (mirrors `include/cge/` structure) |
| `filterGenerator/` | Qt Widgets GUI app for filter testing (Qt6 migration in progress) |
| `examples/` | Sample programs demonstrating library usage |
| `tools/` | Utility programs (Generator, Checker) |
| `docs/` | Documentation (build guides, quick starts, API docs) |

## Build System

Use `tasks.sh` for all operations (`./tasks.sh --help`). Key CMake options:
- `BUILD_FILTER_GENERATOR` (default: OFF) — experimental Qt6 GUI app
- `BUILD_SHARED_LIBS` (default: OFF) — build shared library (.so/.dylib)
- `CMAKE_BUILD_TYPE` — Debug or Release

See `docs/BUILD.md` for detailed build instructions.

## Code Conventions

- **C++11 standard** — no C++14/17/20 features
- Follow `.clang-format` (LLVM-based, 120 col, 4 spaces)
- All core code in `namespace CGE {}`
- Headers: `#pragma once` for include guards
- Platform abstraction via `cgePlatform*.h` files

Language-specific rules in `.github/instructions/code-conventions.instructions.md`.

## Filter Development

- Filters inherit from `CGEImageFilterInterface`
- Use `CGE_SHADER_STRING()` macro for inline GLSL
- Test with both `CGEImageHandler` and real-time rendering
- Every new filter needs header in `include/cge/filters/` and implementation in `src/filters/`

## Documentation

- `README.md` — Project overview, features, quick usage
- `docs/BUILD.md` — Complete build instructions
- `docs/QUICKSTART.md` — 5-minute getting started guide
- `docs/MODERNIZATION.md` — Project upgrade history
- `docs/OPTIMIZATION.md` — Project optimizations and best practices
- `.github/CONTRIBUTING.md` — Contributing workflow
- `.github/RELEASE.md` — Release process and versioning

## Skills

- **Submit PR:** Follow `.github/skills/pr-submit/SKILL.md` to create or update pull requests
- **Review PR:** Follow `.github/skills/pr-review/SKILL.md` to review pull requests

## Behavior Constraints

- **Validation:** After changes, ALWAYS run `./tasks.sh --release --rebuild` to verify.
- **Build System:** When adding source files, CMake `GLOB_RECURSE` auto-discovers them.
- **Commit Policy:** Only commit to feature branches, never force-push to `master`.
- **Completeness:** Implement fully or request clarification — avoid TODOs in committed code.
- **Thread Safety:** Qt and OpenGL calls must be on correct threads.
- **Qt Version:** Always check Qt version at compile time for compatibility (`QT_VERSION >= QT_VERSION_CHECK(6,0,0)`).

## Known Issues & Constraints

- **filterGenerator:** Qt6 migration incomplete (QRegExpValidator, context sharing APIs)
- **Shared Library (macOS):** AGL framework linking issue — use static library
- **OpenGL Warnings:** macOS 10.14+ deprecation warnings are expected (non-blocking)

## Platform Notes

- **macOS:** OpenGL deprecated but still functional; consider Metal in future
- **Windows:** MSVC requires UTF-8 flag and special defines
- **Linux:** Standard OpenGL development packages required
- **CCache:** Automatically enabled if installed for faster recompilation
