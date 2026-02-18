# CGE-Tools Documentation

Welcome to the CGE-Tools documentation!

## 📚 Available Documentation

### Getting Started

- **[QUICKSTART.md](QUICKSTART.md)** - Get up and running in 5 minutes
  - Quick installation guide
  - First example program
  - Common filters usage
  - Troubleshooting tips

### Building & Development

- **[BUILD.md](BUILD.md)** - Comprehensive build instructions
  - Prerequisites for each platform
  - Build configuration options
  - Platform-specific notes
  - Advanced build scenarios
  - Troubleshooting build issues

- **[OPTIMIZATION.md](OPTIMIZATION.md)** - Project optimizations
  - CMake improvements
  - VSCode integration
  - Build scripts and tools
  - Performance tips
  - Comparison with reference projects

### Project History

- **[MODERNIZATION.md](MODERNIZATION.md)** - Modernization journey
  - QMake to CMake migration
  - Qt5 to Qt6 upgrade
  - Code reorganization
  - Build system improvements
  - Lessons learned

## 🔗 Other Resources

### In Root Directory

- **[../README.md](../README.md)** - Main project README
  - Project overview
  - Features and capabilities
  - Quick usage examples
  - Installation instructions

### In .github Directory

- **[../.github/CONTRIBUTING.md](../.github/CONTRIBUTING.md)** - Contributing guidelines
  - How to contribute
  - Code standards
  - Pull request process
  - Compatibility rules

- **[../.github/RELEASE.md](../.github/RELEASE.md)** - Release process
  - Version numbering
  - Release checklist
  - Creating releases

- **[../.github/copilot-instructions.md](../.github/copilot-instructions.md)** - AI Agent instructions
  - Project conventions
  - Code style rules
  - Build constraints

### Examples

- **[../examples/README.md](../examples/README.md)** - Example programs
  - Running examples
  - Creating your own
  - Filter usage demonstrations

## 📖 Documentation Structure

```
cge-tools/
├── README.md                 # Main project overview
├── docs/                     # Detailed documentation
│   ├── README.md            # This file
│   ├── QUICKSTART.md        # Quick start guide
│   ├── BUILD.md             # Build instructions
│   ├── OPTIMIZATION.md      # Optimization guide
│   └── MODERNIZATION.md     # Project history
├── .github/                  # GitHub & AI configurations
│   ├── CONTRIBUTING.md      # Contribution guidelines
│   ├── RELEASE.md           # Release process
│   ├── copilot-instructions.md
│   ├── instructions/        # Code conventions
│   └── skills/              # AI skills (PR submit/review)
└── examples/                 # Example programs
    └── README.md            # Examples guide
```

## 🚀 Recommended Reading Order

### For New Users

1. [QUICKSTART.md](QUICKSTART.md) - Get started immediately
2. [../examples/README.md](../examples/README.md) - Try the examples
3. [BUILD.md](BUILD.md) - Learn about build options

### For Contributors

1. [../.github/CONTRIBUTING.md](../.github/CONTRIBUTING.md) - Read first!
2. [BUILD.md](BUILD.md) - Understand the build system
3. [OPTIMIZATION.md](OPTIMIZATION.md) - Learn the tooling
4. [../.github/copilot-instructions.md](../.github/copilot-instructions.md) - Project conventions

### For Maintainers

1. [../.github/RELEASE.md](../.github/RELEASE.md) - Release process
2. [MODERNIZATION.md](MODERNIZATION.md) - Project evolution
3. [OPTIMIZATION.md](OPTIMIZATION.md) - Infrastructure setup

## 🔍 Finding Information

### By Topic

- **Installation**: QUICKSTART.md, BUILD.md
- **Usage**: ../README.md, ../examples/README.md
- **Contributing**: ../.github/CONTRIBUTING.md
- **Build System**: BUILD.md, OPTIMIZATION.md
- **Project History**: MODERNIZATION.md
- **Release**: ../.github/RELEASE.md

### By Platform

- **macOS**: BUILD.md, QUICKSTART.md
- **Linux**: BUILD.md
- **Windows**: BUILD.md

### By Role

- **User**: QUICKSTART.md, ../README.md
- **Developer**: CONTRIBUTING.md, BUILD.md, OPTIMIZATION.md
- **Maintainer**: RELEASE.md, MODERNIZATION.md

## 💡 Need Help?

- Check the documentation linked above
- Look at [examples](../examples/)
- Open an [issue](https://github.com/wysaid/cge-tools/issues)
- Email: admin@wysaid.org

## 📝 Contributing to Documentation

Found a typo or want to improve the docs? Contributions welcome!

1. Edit the relevant markdown file
2. Run `./tasks.sh --release --rebuild` to test
3. Submit a pull request with your changes

See [../.github/CONTRIBUTING.md](../.github/CONTRIBUTING.md) for details.

---

**Last Updated**: 2026-02-16
