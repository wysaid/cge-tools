# GitHub Copilot Instructions — cge-tools

C++ OpenGL image/video processing library with Qt6 integration.

## Core Constraints

- Never break public API in `include/cge/`.
- Keep Qt code compatible with Qt 6.0+.
- Maintain both standard OpenGL and OpenGL ES support.
- Prefer additive changes; avoid refactoring stable, well-tested filters unless required.
- Do not change default CMake options unless requested.

## Required Workflow

- Use `tasks.sh` for project operations.
- After code changes, run: `./tasks.sh --release --rebuild`.
- New filter code must include both:
	- public header in `include/cge/filters/`
	- implementation in `src/filters/`
- Commit only to feature branches; never force-push `master`.

## References

- Conventions: `.github/instructions/code-conventions.instructions.md`
- Build: `docs/BUILD.md`
- PR submit/review: `.github/skills/pr-submit/SKILL.md`, `.github/skills/pr-review/SKILL.md`