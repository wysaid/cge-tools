# Release Process

Guidelines for creating releases of cge-tools.

## Version Numbering

Follow Semantic Versioning (semver): `MAJOR.MINOR.PATCH`

- **MAJOR**: Breaking API changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, backward compatible

Examples: `1.0.0`, `1.1.0`, `1.1.1`

## Pre-release Versions

- **Alpha**: `1.0.0-alpha.1` - Early testing
- **Beta**: `1.0.0-beta.1` - Feature complete, testing
- **RC**: `1.0.0-rc.1` - Release candidate

## Release Checklist

### 1. Prepare Release

- [ ] Update version in `CMakeLists.txt`:
  ```cmake
  project(cge VERSION 1.1.0 LANGUAGES CXX)
  ```

- [ ] Update `CHANGELOG.md` (if exists) or create release notes

- [ ] Run full test suite:
  ```bash
  ./tasks.sh --release --rebuild
  cd examples/build && ./simple_filter
  ```

- [ ] Format all code:
  ```bash
  ./format-all.sh
  ```

- [ ] Update documentation if needed

- [ ] Check all examples compile and run

### 2. Version Commit

```bash
git add CMakeLists.txt CHANGELOG.md
git commit -m "chore: bump version to 1.1.0"
```

### 3. Create Tag

```bash
# Create annotated tag
git tag -a v1.1.0 -m "Release version 1.1.0

Features:
- Add new vignette filter
- Improve performance of blur filters

Fixes:
- Fix brightness calculation edge case
- Correct memory leak in texture cleanup

See CHANGELOG.md for full details"

# Push commits and tags
git push origin master
git push origin v1.1.0
```

### 4. Create GitHub Release

#### Option A: Using GitHub CLI

```bash
export PAGER=cat

# Create release with notes
gh release create v1.1.0 \
  --title "CGE-Tools v1.1.0" \
  --notes "See CHANGELOG.md for details" \
  --latest

# Or with notes from file
gh release create v1.1.0 \
  --title "CGE-Tools v1.1.0" \
  --notes-file RELEASE_NOTES.md
```

#### Option B: Using GitHub Web UI

1. Go to https://github.com/wysaid/cge-tools/releases/new
2. Select tag: `v1.1.0`
3. Fill in release title: "CGE-Tools v1.1.0"
4. Add release notes (see template below)
5. Upload artifacts (optional):
   - `libcge.a` (Release build)
   - `libcged.a` (Debug build)
   - Documentation PDFs (if any)
6. Check "Set as the latest release"
7. Click "Publish release"

## Release Notes Template

```markdown
# CGE-Tools v1.1.0

Release date: YYYY-MM-DD

## 🎉 Highlights

- Major new feature or improvement

## ✨ New Features

- **Filter**: Added CGEVignetteFilter for vignette effects
- **API**: New method `CGEImageHandler::batchProcessFilters()`
- **Build**: CCache support for faster compilation

## 🐛 Bug Fixes

- Fix brightness calculation overflow for high values (#123)
- Correct memory leak in texture cleanup (#124)
- Fix Qt6 compatibility issue with QOpenGLWidget (#125)

## 🚀 Performance

- 2x faster gaussian blur implementation
- Reduced memory usage in multi-filter pipeline

## 📚 Documentation

- New quick start guide (docs/QUICKSTART.md)
- Updated build instructions for Windows
- API documentation improvements

## 🔧 Maintenance

- Updated to support Qt 6.7+
- Improved CMake configuration
- Better error messages

## ⚠️ Breaking Changes

None in this release.

## 📦 Build Artifacts

- **Static Library (Release)**: libcge.a (3.1 MB)
- **Static Library (Debug)**: libcged.a (25 MB)

## 🔗 Links

- [Full Changelog](https://github.com/wysaid/cge-tools/compare/v1.0.0...v1.1.0)
- [Documentation](https://github.com/wysaid/cge-tools/tree/v1.1.0/docs)
- [Build Instructions](docs/BUILD.md)

## 🙏 Contributors

Thanks to all contributors who made this release possible!

- @contributor1
- @contributor2

---

**Full Changelog**: https://github.com/wysaid/cge-tools/compare/v1.0.0...v1.1.0
```

## Build Artifacts

For major releases, consider providing:

1. **Source Archive**:
   ```bash
   git archive --format=tar.gz --prefix=cge-tools-1.1.0/ v1.1.0 > cge-tools-1.1.0.tar.gz
   ```

2. **Pre-built Libraries** (optional):
   - macOS: `libcge.a` (Release)
   - Linux: `libcge.a` (Release)
   - Windows: `cge.lib` (Release)

3. **Documentation** (if available):
   - API reference (Doxygen HTML)
   - User guide PDF

## Post-Release

### 1. Announce Release

- GitHub Discussions
- Project README badges
- Social media (if applicable)

### 2. Update Development Branch

```bash
# Start next development cycle
git checkout master

# Update version to next development version
# e.g., 1.1.0 -> 1.2.0-dev in CMakeLists.txt if desired
```

### 3. Monitor for Issues

- Watch for bug reports related to the new release
- Be prepared to create patch releases (1.1.1) if critical issues found

## Hotfix Releases

For critical bugs in production:

1. Create hotfix branch from release tag:
   ```bash
   git checkout -b hotfix/1.1.1 v1.1.0
   ```

2. Fix the bug and commit

3. Update version to `1.1.1`

4. Tag and release:
   ```bash
   git tag v1.1.1
   git push origin v1.1.1
   ```

5. Merge hotfix back to master:
   ```bash
   git checkout master
   git merge hotfix/1.1.1
   ```

## Release Frequency

Suggested cadence:
- **Major releases**: When breaking changes are necessary (rare)
- **Minor releases**: Every 2-3 months with new features
- **Patch releases**: As needed for bug fixes

## Version Support

- **Current release**: Full support
- **Previous minor**: Security fixes only
- **Older versions**: Community support

## Related Resources

- [Semantic Versioning](https://semver.org/)
- [Keep a Changelog](https://keepachangelog.com/)
- [GitHub Releases Documentation](https://docs.github.com/en/repositories/releasing-projects-on-github)

## Questions?

Contact project maintainers:
- Email: admin@wysaid.org
- Issues: https://github.com/wysaid/cge-tools/issues
