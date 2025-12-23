#!/usr/bin/env bash
set -euo pipefail

# ---------------------------------------
# Output
# ---------------------------------------
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="$ROOT_DIR/dist/web"
OUT_HTML="$OUT_DIR/index.html"

# ---------------------------------------
# Raylib web build location (cache)
# ---------------------------------------
RAYLIB_WORKDIR="$ROOT_DIR/build/raylib_web"
RAYLIB_SRC_DIR="$RAYLIB_WORKDIR/raylib"
RAYLIB_SRC_REPO="${RAYLIB_SRC_REPO:-https://github.com/raysan5/raylib.git}"
RAYLIB_GIT_REF="${RAYLIB_GIT_REF:-master}" # optionally set to a tag like 5.0

# ---------------------------------------
# Emscripten environment
# ---------------------------------------

if ! command -v em++ >/dev/null 2>&1; then
  echo "ERROR: em++ not found."
  echo "Install/activate Emscripten (emsdk). Example:"
  echo "  git clone https://github.com/emscripten-core/emsdk.git"
  echo "  cd emsdk && ./emsdk install latest && ./emsdk activate latest"
  echo "  source ./emsdk_env.sh"
  echo ""
  echo "Or set EMSDK=/path/to/emsdk so this script can source it."
  exit 1
fi

# ---------------------------------------
# Tools
# ---------------------------------------
if ! command -v git >/dev/null 2>&1; then
  echo "ERROR: git is required to fetch raylib source."
  exit 1
fi

if ! command -v make >/dev/null 2>&1; then
  echo "ERROR: make is required."
  exit 1
fi

# ---------------------------------------
# Fetch raylib source (cached)
# ---------------------------------------
mkdir -p "$RAYLIB_WORKDIR"

if [[ ! -d "$RAYLIB_SRC_DIR/.git" ]]; then
  echo "Fetching raylib source into: $RAYLIB_SRC_DIR"
  rm -rf "$RAYLIB_SRC_DIR"
  git clone --depth 1 "$RAYLIB_SRC_REPO" "$RAYLIB_SRC_DIR"
else
  echo "Using existing raylib source at: $RAYLIB_SRC_DIR"
fi

# Optionally checkout a specific ref
(
  cd "$RAYLIB_SRC_DIR"
  # Fetch minimal history if needed for non-master refs
  if [[ "$RAYLIB_GIT_REF" != "master" ]]; then
    git fetch --depth 1 origin "$RAYLIB_GIT_REF" || true
  fi
  git checkout -q "$RAYLIB_GIT_REF" || true
)

# ---------------------------------------
# Build raylib for web (Emscripten)
# ---------------------------------------
echo "Building raylib for Web (PLATFORM_WEB)..."
(
  cd "$RAYLIB_SRC_DIR/src"
  # Clean ensures we don't accidentally link a native build
  make clean >/dev/null 2>&1 || true

  # raylib's Makefile auto-detects EMSCRIPTEN when emcc/em++ are in PATH.
  # This builds: libraylib.a for web.
  make -j"$(sysctl -n hw.ncpu 2>/dev/null || echo 4)" PLATFORM=PLATFORM_WEB
)

RAYLIB_INCLUDE_DIR="$RAYLIB_SRC_DIR/src"
RAYLIB_WEB_LIB="$RAYLIB_SRC_DIR/src/libraylib.web.a"

# ---------------------------------------
# Gather project sources (exclude scripts/)
# ---------------------------------------
SOURCES=()

while IFS= read -r file; do
  SOURCES+=("$file")
done < <(
  cd "$ROOT_DIR"
  find . -type f -name "*.cpp" -print | sed 's|^\./||'
)

if [[ ${#SOURCES[@]} -eq 0 ]]; then
  echo "ERROR: No .cpp sources found."
  exit 1
fi

# ---------------------------------------
# Build output directory
# ---------------------------------------
mkdir -p "$OUT_DIR"

echo "Compiling project to: $OUT_HTML"
echo "Sources: ${#SOURCES[@]} file(s)"

# ---------------------------------------
# Compile + link with em++
# ---------------------------------------
# Notes:
# - --preload-file bundles /res into a .data file and mounts it at runtime.
# - USE_GLFW=3 is commonly used for raylib web.
# - WebGL2 requested.
# - ALLOW_MEMORY_GROWTH avoids fixed heap pain.

em++ \
  -std=c++17 -O2 \
  -DPLATFORM_WEB \
  -I"$ROOT_DIR" \
  -I"$ROOT_DIR/suicune_src" \
  -I"$ROOT_DIR/example_src" \
  -I"$RAYLIB_INCLUDE_DIR" \
  "${SOURCES[@]/#/$ROOT_DIR/}" \
  "$RAYLIB_WEB_LIB" \
  -sUSE_GLFW=3 \
  -sALLOW_MEMORY_GROWTH=1 \
  -sMIN_WEBGL_VERSION=2 \
  -sMAX_WEBGL_VERSION=2 \
  --shell-file "scripts/web-shell.html" \
  --preload-file "$ROOT_DIR/res@res" \
  -o "$OUT_HTML"

echo "Web build complete."
echo "Output folder: $OUT_DIR"
echo "Open via a local server (required for .data/.wasm):"
echo "  cd \"$OUT_DIR\" && python3 -m http.server 8080"
echo "Then visit:"
echo "  http://localhost:8080/index.html"
