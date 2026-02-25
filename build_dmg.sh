#!/usr/bin/env bash
# build_dmg.sh — Deploy Qt deps and package .app bundle(s) into .dmg files.
#
# Default (no args):
#   1. Runs macdeployqt on filterGenerator.app and filterRuleTest.app found in
#      ./build/filterGenerator/.
#   2. Resolves symlinks in each app's Frameworks/ directory.
#   3. Packages each .app into a .dmg and writes them into ./build/.
#
# Single-app mode:
#   ./build_dmg.sh --app-path <path/to/App.app> --output <path/to/output.dmg>
#   (macdeployqt is NOT run automatically — caller is responsible)
#
# Options:
#   --app-path PATH   Path to the .app bundle (single-app mode)
#   --output   PATH   Destination .dmg file path (single-app mode)
#   --volname  NAME   Volume name when the DMG is mounted
#                     (default: basename of the .app without extension)
#   --no-deploy       Skip macdeployqt step (default mode only)
#   --no-sign         Skip ad-hoc code signing step
#   -h, --help        Show this help and exit

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

APP_PATH=""
OUTPUT_PATH=""
VOLNAME=""
SKIP_SIGN=false
SKIP_DEPLOY=false

# ── Argument parsing ─────────────────────────────────────────────────────────
while [[ $# -gt 0 ]]; do
    case "$1" in
    --app-path)
        APP_PATH="$2"
        shift 2
        ;;
    --output)
        OUTPUT_PATH="$2"
        shift 2
        ;;
    --volname)
        VOLNAME="$2"
        shift 2
        ;;
    --no-deploy)
        SKIP_DEPLOY=true
        shift
        ;;
    --no-sign)
        SKIP_SIGN=true
        shift
        ;;
    -h | --help)
        sed -n '2,22p' "$0" | sed 's/^# \?//'
        exit 0
        ;;
    *)
        echo "Unknown argument: $1" >&2
        exit 1
        ;;
    esac
done

# ── Locate macdeployqt ────────────────────────────────────────────────────────
find_macdeployqt() {
    local _BIN
    _BIN="$(command -v macdeployqt 2>/dev/null || true)"
    if [[ -z "$_BIN" ]] && [[ -n "${QTDIR:-}" ]]; then
        _BIN="$QTDIR/bin/macdeployqt"
    fi
    if [[ -z "$_BIN" ]] && [[ -n "${QT_DIR:-}" ]]; then
        _BIN="$QT_DIR/bin/macdeployqt"
    fi
    # Homebrew fallback
    if [[ -z "$_BIN" ]]; then
        _BIN="$(find /opt/homebrew /usr/local -maxdepth 5 -name macdeployqt -type f 2>/dev/null | head -1 || true)"
    fi
    echo "$_BIN"
}

# ── Resolve symlinks inside Frameworks/ (hdiutil can choke on broken symlinks) ──
resolve_frameworks_symlinks() {
    local _FW_DIR="$1/Contents/Frameworks"
    [[ -d "$_FW_DIR" ]] || return 0
    local _F _REAL
    for _F in "$_FW_DIR"/*; do
        if [[ -L "$_F" ]]; then
            _REAL="$(realpath "$_F")"
            rm "$_F"
            cp -r "$_REAL" "$_F"
            echo "  resolved symlink: $(basename "$_F")"
        fi
    done
}

# ── Run macdeployqt on a single .app ─────────────────────────────────────────
deploy_app() {
    local _APP="$1"
    local _QDEPLOY
    _QDEPLOY="$(find_macdeployqt)"

    if [[ -z "$_QDEPLOY" ]] || [[ ! -x "$_QDEPLOY" ]]; then
        echo "Error: macdeployqt not found. Set QTDIR or add it to PATH." >&2
        return 1
    fi

    echo "→ macdeployqt: $_APP"
    "$_QDEPLOY" "$_APP" -verbose=1

    echo "  Resolving Frameworks symlinks..."
    resolve_frameworks_symlinks "$_APP"
}

# ── Core DMG packaging function ───────────────────────────────────────────────
# package_dmg <app_path> <output_path> [volname]
package_dmg() {
    local _APP="$1"
    local _OUT="$2"
    local _VOL="${3:-$(basename "$_APP" .app)}"

    if [[ ! -d "$_APP" ]]; then
        echo "Error: .app bundle not found at: $_APP" >&2
        return 1
    fi

    _APP="$(cd "$_APP" && pwd)"   # absolute path
    mkdir -p "$(dirname "$_OUT")"

    # Remove dangling .DS_Store symlinks left by macdeployqt
    find "$_APP" -name '.DS_Store' -type l -delete 2>/dev/null || true

    # Ad-hoc code sign
    if ! $SKIP_SIGN; then
        echo "→ Signing (ad-hoc): $_APP"
        if ! codesign --force --deep --sign - "$_APP"; then
            echo "Warning: codesign failed — continuing without signing." >&2
        else
            echo "  Signing complete."
        fi
    fi

    # Remove pre-existing DMG so hdiutil does not prompt
    [[ -f "$_OUT" ]] && rm -f "$_OUT"

    echo "→ Creating DMG: $_OUT"
    hdiutil create \
        -volname "$_VOL" \
        -srcfolder "$_APP" \
        -ov \
        -format ULMO \
        "$_OUT"

    echo "✓ DMG ready: $_OUT ($(du -sh "$_OUT" | cut -f1))"
}

# ── Default mode (no --app-path / --output provided) ─────────────────────────
if [[ -z "$APP_PATH" ]] && [[ -z "$OUTPUT_PATH" ]]; then
    BUILD_DIR="$SCRIPT_DIR/build"
    FG_DIR="$BUILD_DIR/filterGenerator"

    echo "=== Default mode: packaging Release builds from $FG_DIR ==="

    FAILED=0
    for APP in \
        "$FG_DIR/filterGenerator.app" \
        "$FG_DIR/filterRuleTest.app"
    do
        if [[ ! -d "$APP" ]]; then
            echo "Skipping (not found): $APP"
            continue
        fi

        APP_BASE="$(basename "$APP" .app)"

        # Deploy Qt deps unless explicitly skipped
        if ! $SKIP_DEPLOY; then
            deploy_app "$APP" || { FAILED=1; continue; }
        fi

        package_dmg "$APP" "$BUILD_DIR/${APP_BASE}.dmg" || FAILED=1
    done

    if [[ $FAILED -eq 1 ]]; then
        echo "One or more DMG builds failed." >&2
        exit 1
    fi
    exit 0
fi

# ── Single-app mode ───────────────────────────────────────────────────────────
if [[ -z "$APP_PATH" ]] || [[ -z "$OUTPUT_PATH" ]]; then
    echo "Error: --app-path and --output must be specified together." >&2
    exit 1
fi

# In single-app mode macdeployqt is the caller's responsibility (e.g. CI).
package_dmg "$APP_PATH" "$OUTPUT_PATH" "$VOLNAME"
