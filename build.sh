#!/usr/bin/env bash
set -euo pipefail

# Build config
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TOOLCHAIN_FILE="$PROJECT_ROOT/cmake/toolchains/mingw-w64-x86_64.cmake"

# Linux build
echo "[+] Building native Linux binary..."
cmake -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/build/linux" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release
cmake --build "$PROJECT_ROOT/build/linux"

# Windows cross-compile
echo "[+] Building Windows .exe..."
cmake -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/build/windows" -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DCMAKE_BUILD_TYPE=Release
cmake --build "$PROJECT_ROOT/build/windows"

echo "[✓] Done."

if [[ "${1:-}" == "clean" ]]; then
    echo "[*] Cleaning builds..."
    rm -rf "$PROJECT_ROOT/build"
    exit 0
fi
