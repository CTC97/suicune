#!/usr/bin/env bash
set -euo pipefail

# Define colors
RESET="\033[0m"
MEDIUM_BLUE="\033[38;5;24m"  # #4A6FA5
LIGHT_BLUE="\033[38;5;153m"  # #E6EEF7
PURPLE="\033[38;5;139m"  # #B7AEC8
LIGHT_PURPLE="\033[38;5;140m"  # #9E97B8

usage() {
  echo -e "${PURPLE}Usage: suicune_dist --mac [name] | --web${RESET}"
  exit 1
}

if [[ $# -lt 1 ]]; then
  usage
fi

MODE="$1"
shift

# ---------------------------------------
# --mac: build macOS .app bundle
# ---------------------------------------
if [[ "$MODE" == "--mac" ]]; then
  echo -e "${MEDIUM_BLUE}Building...${RESET}"
  make > /dev/null
  echo -e "\t${LIGHT_BLUE}Done.${RESET}"

  DIST_NAME="${1:-${DIST_NAME:-dist}}"
  BIN_PATH="build/executable"
  RES_DIR="${RES_DIR:-res}"
  DIST_DIR="${DIST_DIR:-dist}"

  APP_DIR="$DIST_DIR/$DIST_NAME.app"
  MACOS_DIR="$APP_DIR/Contents/MacOS"
  RSRC_DIR="$APP_DIR/Contents/Resources"

  if [[ ! -f "$BIN_PATH" ]]; then
    echo -e "${PURPLE}error: binary not found at '$BIN_PATH' (build it first)${RESET}" >&2
    exit 1
  fi

  if [[ ! -d "$RES_DIR" ]]; then
    echo -e "${PURPLE}error: resources folder not found at '$RES_DIR'${RESET}" >&2
    exit 1
  fi

  echo -e "${MEDIUM_BLUE}Creating bundle...${RESET}"
  rm -rf "$APP_DIR"
  mkdir -p "$MACOS_DIR" "$RSRC_DIR"
  echo -e "\t${LIGHT_BLUE}Directories created.${RESET}"

  echo -e "${MEDIUM_BLUE}Copying binary...${RESET}"
  cp "$BIN_PATH" "$MACOS_DIR/$DIST_NAME"
  chmod +x "$MACOS_DIR/$DIST_NAME"
  echo -e "\t${LIGHT_BLUE}Binary copied.${RESET}"

  echo -e "${MEDIUM_BLUE}Copying resources...${RESET}"
  cp -R "$RES_DIR" "$RSRC_DIR/$RES_DIR"
  echo -e "\t${LIGHT_BLUE}Resources copied.${RESET}"

  echo -e "${MEDIUM_BLUE}Creating Info.plist...${RESET}"
  cat > "$APP_DIR/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleName</key><string>$DIST_NAME</string>
  <key>CFBundleExecutable</key><string>$DIST_NAME</string>
  <key>CFBundleIdentifier</key><string>com.example.$(echo "$DIST_NAME" | tr '[:upper:]' '[:lower:]')</string>
  <key>CFBundlePackageType</key><string>APPL</string>
  <key>CFBundleShortVersionString</key><string>1.0</string>
  <key>CFBundleVersion</key><string>1.0</string>
</dict>
</plist>
EOF
  echo -e "\t${LIGHT_BLUE}Info.plist created.${RESET}"

  echo -e "${MEDIUM_BLUE}.app created:${RESET}"
  echo -e "\t${LIGHT_BLUE}.. $APP_DIR${RESET}"

# ---------------------------------------
# --web: build WebAssembly via Emscripten
# ---------------------------------------
elif [[ "$MODE" == "--web" ]]; then
  ROOT_DIR="$(pwd)"
  OUT_DIR="$ROOT_DIR/dist/web"
  OUT_HTML="$OUT_DIR/index.html"

  RAYLIB_WORKDIR="$ROOT_DIR/build/raylib_web"
  RAYLIB_SRC_DIR="$RAYLIB_WORKDIR/raylib"
  RAYLIB_SRC_REPO="${RAYLIB_SRC_REPO:-https://github.com/raysan5/raylib.git}"
  RAYLIB_GIT_REF="${RAYLIB_GIT_REF:-master}"

  # Locate web-shell.html relative to this script (works both installed via
  # brew and run directly from the repo's scripts/ folder)
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  WEB_SHELL="$SCRIPT_DIR/../share/suicune/web-shell.html"
  if [[ ! -f "$WEB_SHELL" ]]; then
    WEB_SHELL="$SCRIPT_DIR/web-shell.html"
  fi
  if [[ ! -f "$WEB_SHELL" ]]; then
    echo -e "${PURPLE}error: web-shell.html not found${RESET}" >&2
    exit 1
  fi

  if ! command -v em++ >/dev/null 2>&1; then
    echo -e "${PURPLE}ERROR: em++ not found.${RESET}"
    echo "Install/activate Emscripten (emsdk). Example:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk && ./emsdk install latest && ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    exit 1
  fi

  if ! command -v git >/dev/null 2>&1; then
    echo -e "${PURPLE}ERROR: git is required to fetch raylib source.${RESET}" >&2
    exit 1
  fi

  if ! command -v make >/dev/null 2>&1; then
    echo -e "${PURPLE}ERROR: make is required.${RESET}" >&2
    exit 1
  fi

  mkdir -p "$RAYLIB_WORKDIR"

  if [[ ! -d "$RAYLIB_SRC_DIR/.git" ]]; then
    echo -e "${MEDIUM_BLUE}Fetching raylib source...${RESET}"
    rm -rf "$RAYLIB_SRC_DIR"
    git clone --depth 1 "$RAYLIB_SRC_REPO" "$RAYLIB_SRC_DIR"
    echo -e "\t${LIGHT_BLUE}Done.${RESET}"
  else
    echo -e "${LIGHT_BLUE}Using cached raylib source.${RESET}"
  fi

  (
    cd "$RAYLIB_SRC_DIR"
    if [[ "$RAYLIB_GIT_REF" != "master" ]]; then
      git fetch --depth 1 origin "$RAYLIB_GIT_REF" || true
    fi
    git checkout -q "$RAYLIB_GIT_REF" || true
  )

  echo -e "${MEDIUM_BLUE}Building raylib for Web...${RESET}"
  (
    cd "$RAYLIB_SRC_DIR/src"
    make clean >/dev/null 2>&1 || true
    make -j"$(sysctl -n hw.ncpu 2>/dev/null || echo 4)" PLATFORM=PLATFORM_WEB
  )
  echo -e "\t${LIGHT_BLUE}Done.${RESET}"

  RAYLIB_INCLUDE_DIR="$RAYLIB_SRC_DIR/src"
  RAYLIB_WEB_LIB="$RAYLIB_SRC_DIR/src/libraylib.web.a"

  SOURCES=()
  while IFS= read -r file; do
    SOURCES+=("$file")
  done < <(
    cd "$ROOT_DIR"
    find . -type f -name "*.cpp" -print | sed 's|^\./||'
  )

  if [[ ${#SOURCES[@]} -eq 0 ]]; then
    echo -e "${PURPLE}ERROR: No .cpp sources found.${RESET}" >&2
    exit 1
  fi

  mkdir -p "$OUT_DIR"

  echo -e "${MEDIUM_BLUE}Compiling to WebAssembly...${RESET}"
  echo -e "\t${LIGHT_BLUE}${#SOURCES[@]} source file(s)${RESET}"

  em++ \
    -std=c++17 -O2 \
    -DPLATFORM_WEB \
    -I"$ROOT_DIR" \
    -I"$ROOT_DIR/suicune_src" \
    -I"$ROOT_DIR/src" \
    -I"$RAYLIB_INCLUDE_DIR" \
    "${SOURCES[@]/#/$ROOT_DIR/}" \
    "$RAYLIB_WEB_LIB" \
    -sUSE_GLFW=3 \
    -sALLOW_MEMORY_GROWTH=1 \
    -sMIN_WEBGL_VERSION=2 \
    -sMAX_WEBGL_VERSION=2 \
    --shell-file "$WEB_SHELL" \
    --preload-file "$ROOT_DIR/res@res" \
    -o "$OUT_HTML"

  echo -e "${MEDIUM_BLUE}Web build complete:${RESET}"
  echo -e "\t${LIGHT_BLUE}.. $OUT_DIR${RESET}"
  echo -e "\t${LIGHT_BLUE}Serve with: cd \"$OUT_DIR\" && python3 -m http.server 8080${RESET}"

else
  usage
fi
