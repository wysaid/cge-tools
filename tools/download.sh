#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────────────────
# tools/download.sh  —  Download CGE Tools from the latest GitHub Release
#
# Usage:
#   bash tools/download.sh           # interactive selection menu
#   bash tools/download.sh --auto    # download packages for the current platform
#   bash tools/download.sh --all     # download all available packages
#   bash tools/download.sh --help    # show help
#
# Requirements: curl, python3 (both ship with macOS; standard on Linux)
# Downloaded files are saved to:  tools/downloads/
# ─────────────────────────────────────────────────────────────────────────────

set -uo pipefail

REPO="wysaid/cge-tools"
RELEASES_URL="https://github.com/${REPO}/releases"
API_URL="https://api.github.com/repos/${REPO}/releases/latest"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOWNLOAD_DIR="${SCRIPT_DIR}/downloads"

# ─── Colour helpers ───────────────────────────────────────────────────────────
if [[ -t 1 ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    CYAN='\033[0;36m'
    BOLD='\033[1m'
    RESET='\033[0m'
else
    RED=''
    GREEN=''
    YELLOW=''
    CYAN=''
    BOLD=''
    RESET=''
fi

die() {
    printf "${RED}Error: %s${RESET}\n" "$1" >&2
    exit 1
}
info() { printf "${CYAN}%s${RESET}\n" "$1"; }
ok() { printf "${GREEN}%s${RESET}\n" "$1"; }
warn() { printf "${YELLOW}%s${RESET}\n" "$1"; }

# ─── Argument parsing ─────────────────────────────────────────────────────────
MODE="interactive"
while [[ $# -gt 0 ]]; do
    case "$1" in
    --all)
        MODE="all"
        shift
        ;;
    --auto)
        MODE="auto"
        shift
        ;;
    --help | -h)
        echo "Usage: $(basename "$0") [--all | --auto | --help]"
        echo
        echo "  (no args)  Launch interactive selection menu"
        echo "  --auto     Download packages matching the current platform"
        echo "  --all      Download every available package"
        echo
        echo "Files are saved to: tools/downloads/"
        echo "Releases page:      ${RELEASES_URL}"
        exit 0
        ;;
    *) die "Unknown argument: $1  (run with --help for usage)" ;;
    esac
done

# ─── Dependency check ─────────────────────────────────────────────────────────
command -v curl >/dev/null 2>&1 || die "'curl' is required. Please install it and retry."
command -v python3 >/dev/null 2>&1 || die "'python3' is required. Please install it and retry."

# ─── Platform detection ───────────────────────────────────────────────────────
detect_platform() {
    local os arch
    os="$(uname -s 2>/dev/null || echo unknown)"
    arch="$(uname -m 2>/dev/null || echo unknown)"
    case "$os" in
    Darwin)
        [[ "$arch" == "arm64" ]] && echo "macos-arm64" || echo "macos-x86_64"
        ;;
    Linux)
        if [[ -f /etc/fedora-release ]]; then
            echo "linux-x86_64-fedora"
        else
            echo "linux-x86_64-ubuntu22"
        fi
        ;;
    MINGW* | MSYS* | CYGWIN*)
        echo "windows-x64"
        ;;
    *)
        echo "unknown"
        ;;
    esac
}

PLATFORM="$(detect_platform)"

# ─── Fetch latest release ─────────────────────────────────────────────────────
info "Fetching latest release information from GitHub..."

RELEASE_JSON="$(curl -sL --max-time 30 "$API_URL")" ||
    die "Network error — could not reach the GitHub API. Check your connection."

# Python script to validate and parse the JSON (uses only double-quote string
# literals inside so the whole block can be single-quoted for bash).
PARSE_SCRIPT='
import json, sys

try:
    data = json.load(sys.stdin)
except Exception as e:
    sys.stderr.write("JSON parse error: " + str(e) + "\n")
    sys.exit(1)

if "message" in data and "assets" not in data:
    sys.stderr.write("GitHub API error: " + data["message"] + "\n")
    sys.exit(2)

tag = data.get("tag_name", "unknown")
print("TAG:" + tag)

for a in sorted(data.get("assets", []), key=lambda x: x["name"]):
    name = a["name"]
    url  = a["browser_download_url"]
    size = a.get("size", 0)
    if size >= 1048576:
        size_str = "{:.1f} MB".format(size / 1048576)
    elif size >= 1024:
        size_str = "{:.1f} KB".format(size / 1024)
    else:
        size_str = "{} B".format(size)
    print(name + "|" + url + "|" + size_str)
'

PARSED="$(echo "$RELEASE_JSON" | python3 -c "$PARSE_SCRIPT")" ||
    die "Could not parse release data. Visit ${RELEASES_URL} to download manually."

TAG="$(echo "$PARSED" | grep '^TAG:' | cut -d: -f2-)"
ASSETS_RAW="$(echo "$PARSED" | grep -v '^TAG:')"
ASSET_COUNT="$(echo "$ASSETS_RAW" | grep -c '|' || echo 0)"

[[ "$ASSET_COUNT" -gt 0 ]] ||
    die "No downloadable assets found in the latest release.\nVisit: ${RELEASES_URL}"

# ─── Build arrays ─────────────────────────────────────────────────────────────
declare -a NAMES=() URLS=() SIZE_STRS=() PKG_GROUPS=()

while IFS='|' read -r name url size_str; do
    [[ -z "$name" ]] && continue
    NAMES+=("$name")
    URLS+=("$url")
    SIZE_STRS+=("$size_str")
    if [[ "$name" == *"macos-arm64"* ]]; then
        PKG_GROUPS+=("macOS  |  Apple Silicon (arm64)")
    elif [[ "$name" == *"macos-x86_64"* ]]; then
        PKG_GROUPS+=("macOS  |  Intel (x86_64)")
    elif [[ "$name" == *"ubuntu"* ]]; then
        PKG_GROUPS+=("Linux  |  Ubuntu 22.04 (x86_64)")
    elif [[ "$name" == *"fedora"* ]]; then
        PKG_GROUPS+=("Linux  |  Fedora (x86_64)")
    elif [[ "$name" == *"windows"* ]]; then
        PKG_GROUPS+=("Windows  |  x64")
    else
        PKG_GROUPS+=("Other")
    fi
done <<<"$ASSETS_RAW"

TOTAL="${#NAMES[@]}"
[[ "$TOTAL" -gt 0 ]] || die "No release assets could be parsed."

# ─── Print table ──────────────────────────────────────────────────────────────
echo
printf "${BOLD}CGE Tools — Package Downloader${RESET}\n"
printf "Release : ${YELLOW}%s${RESET}\n" "$TAG"
printf "Page    : %s\n" "$RELEASES_URL"
echo

PREV_GROUP=""
for ((idx = 0; idx < TOTAL; idx++)); do
    g="${PKG_GROUPS[$idx]}"
    if [[ "$g" != "$PREV_GROUP" ]]; then
        [[ -n "$PREV_GROUP" ]] && echo
        printf "  ${BOLD}%s${RESET}\n" "$g"
        PREV_GROUP="$g"
    fi
    HINT=""
    if [[ "$PLATFORM" != "unknown" && "${NAMES[$idx]}" == *"$PLATFORM"* ]]; then
        HINT="${GREEN}  ← your platform${RESET}"
    fi
    printf "    ${CYAN}[%2d]${RESET}  %-62s %s%b\n" \
        $((idx + 1)) "${NAMES[$idx]}" "(${SIZE_STRS[$idx]})" "$HINT"
done

echo
printf '%s\n' "────────────────────────────────────────────────────────────────────"

# ─── Resolve selected indices ─────────────────────────────────────────────────
declare -a SELECTED=()

if [[ "$MODE" == "all" ]]; then
    for ((idx = 0; idx < TOTAL; idx++)); do SELECTED+=($idx); done

elif [[ "$MODE" == "auto" ]]; then
    [[ "$PLATFORM" == "unknown" ]] &&
        die "Could not detect your platform. Use interactive mode instead."
    for ((idx = 0; idx < TOTAL; idx++)); do
        [[ "${NAMES[$idx]}" == *"$PLATFORM"* ]] && SELECTED+=($idx)
    done
    [[ ${#SELECTED[@]} -gt 0 ]] ||
        die "No packages found for your platform (${PLATFORM})."

else
    # ── Interactive menu ────────────────────────────────────────────────────
    echo
    printf "  ${BOLD}[a]${RESET}  Download ALL packages\n"
    if [[ "$PLATFORM" != "unknown" ]]; then
        printf "  ${BOLD}[r]${RESET}  Download recommended for your platform  ${YELLOW}(%s)${RESET}\n" "$PLATFORM"
    fi
    printf "  ${BOLD}[q]${RESET}  Quit\n"
    echo
    printf "Enter selection (number(s) separated by space or comma, 'a', 'r', or 'q'): "
    read -r SELECTION || SELECTION="q"
    echo

    local_sel="$(echo "$SELECTION" | tr '[:upper:]' '[:lower:]')"
    case "$local_sel" in
    q | quit | exit | "")
        info "Exiting."
        exit 0
        ;;
    a | all)
        for ((idx = 0; idx < TOTAL; idx++)); do SELECTED+=($idx); done
        ;;
    r | recommended)
        [[ "$PLATFORM" == "unknown" ]] &&
            die "Cannot detect your platform. Please select manually."
        for ((idx = 0; idx < TOTAL; idx++)); do
            [[ "${NAMES[$idx]}" == *"$PLATFORM"* ]] && SELECTED+=($idx)
        done
        [[ ${#SELECTED[@]} -gt 0 ]] ||
            die "No packages found for platform: ${PLATFORM}"
        ;;
    *)
        for token in ${SELECTION//,/ }; do
            if [[ "$token" =~ ^[0-9]+$ ]]; then
                n=$((token - 1))
                if ((n >= 0 && n < TOTAL)); then
                    SELECTED+=($n)
                else
                    die "Number out of range: ${token} (valid: 1–${TOTAL})"
                fi
            else
                die "Unrecognized input: '${token}'.  Enter numbers, 'a', 'r', or 'q'."
            fi
        done
        ;;
    esac
fi

[[ ${#SELECTED[@]} -gt 0 ]] || die "No packages selected."

# ─── Download ─────────────────────────────────────────────────────────────────
mkdir -p "$DOWNLOAD_DIR"
info "Saving files to: ${DOWNLOAD_DIR}/"
echo

FAIL=0
for idx in "${SELECTED[@]}"; do
    name="${NAMES[$idx]}"
    url="${URLS[$idx]}"
    dest="${DOWNLOAD_DIR}/${name}"

    printf "${BOLD}Downloading${RESET} %s ...\n" "$name"
    if curl -L --progress-bar --max-time 600 --retry 3 -o "$dest" "$url"; then
        ok "  Saved → ${dest}"
    else
        warn "  FAILED: ${name}"
        rm -f "$dest"
        FAIL=$((FAIL + 1))
    fi
    echo
done

# ─── Summary ──────────────────────────────────────────────────────────────────
DONE=$((${#SELECTED[@]} - FAIL))
if ((FAIL == 0)); then
    ok "All ${DONE} file(s) downloaded successfully."
    ok "Location: ${DOWNLOAD_DIR}/"
else
    warn "${DONE} file(s) downloaded, ${FAIL} failed."
    exit 1
fi
