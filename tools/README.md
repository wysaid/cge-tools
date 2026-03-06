# CGE Tools — Pre-built Binaries

> [!WARNING]
> **The binaries in `mac/`, `ubuntu/`, and `windows/` subdirectories are outdated and no longer maintained.**
> They are kept only for historical reference. Use the method below to get the latest version.

## Download Latest Release

Get the latest pre-built binaries from the **GitHub Releases page**:

👉 **[https://github.com/wysaid/cge-tools/releases](https://github.com/wysaid/cge-tools/releases)**

### Quick Download Script

Use [`download.sh`](./download.sh) in this directory to fetch packages interactively:

```bash
# Interactive menu — pick one or more packages
bash tools/download.sh

# Auto-detect your platform and download matching packages
bash tools/download.sh --auto

# Download every available package at once
bash tools/download.sh --all
```

Files are saved to `tools/downloads/`.
The script requires `curl` and `python3` (both ship with macOS and are standard on Linux).

### Available Packages per Release

| Platform | File | Contents |
|---|---|---|
| macOS (Apple Silicon / arm64) | `filterGenerator-macos-arm64.dmg` | FilterGenerator |
| macOS (Apple Silicon / arm64) | `filterRuleTest-macos-arm64.dmg` | FilterRuleTest |
| macOS (Intel / x86_64) | `filterGenerator-macos-x86_64.dmg` | FilterGenerator |
| macOS (Intel / x86_64) | `filterRuleTest-macos-x86_64.dmg` | FilterRuleTest |
| Linux Ubuntu 22.04 (x86_64) | `filterGenerator-linux-x86_64-ubuntu22.AppImage` | FilterGenerator |
| Linux Ubuntu 22.04 (x86_64) | `filterRuleTest-linux-x86_64-ubuntu22.AppImage` | FilterRuleTest |
| Linux Fedora (x86_64) | `filterGenerator-linux-x86_64-fedora.AppImage` | FilterGenerator |
| Linux Fedora (x86_64) | `filterRuleTest-linux-x86_64-fedora.AppImage` | FilterRuleTest |
| Windows x64 (MSVC 2022) | `filterGenerator-windows-x64.zip` | FilterGenerator + FilterRuleTest |

## Building from Source

If no pre-built binary is available for your platform, see the
[Build Documentation](../docs/BUILD.md) or the [Quick Start Guide](../docs/QUICKSTART.md).
