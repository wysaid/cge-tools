#!/bin/bash
# Generate icns from ico files on macOS
set -e
cd "$(dirname "$0")"

gen_icns() {
    local ICO="$1"
    local BASE="${ICO%.ico}"
    local ICNS="${BASE}.icns"

    echo "Generating ${ICNS} from ${ICO}..."
    # Convert ico -> icns directly via sips (no upscaling needed)
    sips -s format icns "$ICO" --out "$ICNS"
    echo "Done: $ICNS"
}

gen_icns "filterGenerator.ico"
gen_icns "filterRuleTest.ico"
