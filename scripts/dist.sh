#!/usr/bin/env bash
set -euo pipefail

# Define colors
RESET="\033[0m"
MEDIUM_BLUE="\033[38;5;24m"  # #4A6FA5
LIGHT_BLUE="\033[38;5;153m"  # #E6EEF7
PURPLE="\033[38;5;139m"  # #B7AEC8
LIGHT_PURPLE="\033[38;5;140m"  # #9E97B8

echo -e "${MEDIUM_BLUE}Building...${RESET}"
make > /dev/null
echo -e "\t${LIGHT_BLUE}Done.${RESET}"

# ---- config ----
DIST_NAME="${1:-${DIST_NAME:-dist}}"  # Use the first argument, or environment variable, or default
BIN_PATH="build/build"  # hardcoded binary path
RES_DIR="${RES_DIR:-res}"              # your resources folder
DIST_DIR="${DIST_DIR:-dist}"           # output folder

APP_DIR="$DIST_DIR/$DIST_NAME.app"
MACOS_DIR="$APP_DIR/Contents/MacOS"
RSRC_DIR="$APP_DIR/Contents/Resources"

# ---- checks ----
if [[ ! -f "$BIN_PATH" ]]; then
  echo -e "${PURPLE}error: binary not found at '$BIN_PATH' (build it first)${RESET}" >&2
  exit 1
fi

if [[ ! -d "$RES_DIR" ]]; then
  echo -e "${PURPLE}error: resources folder not found at '$RES_DIR'${RESET}" >&2
  exit 1
fi

# ---- create bundle ----
echo -e "${MEDIUM_BLUE}Creating bundle...${RESET}"
rm -rf "$APP_DIR"
mkdir -p "$MACOS_DIR" "$RSRC_DIR"
echo -e "\t${LIGHT_BLUE}Directories created.${RESET}"

# Copy binary into the bundle (named exactly CFBundleExecutable)
echo -e "${MEDIUM_BLUE}Copying binary...${RESET}"
cp "$BIN_PATH" "$MACOS_DIR/$DIST_NAME"
chmod +x "$MACOS_DIR/$DIST_NAME"
echo -e "\t${LIGHT_BLUE}Binary copied.${RESET}"

# Copy resources into Contents/Resources/res
echo -e "${MEDIUM_BLUE}Copying resources...${RESET}"
cp -R "$RES_DIR" "$RSRC_DIR/$RES_DIR"
echo -e "\t${LIGHT_BLUE}Resources copied.${RESET}"

# Minimal Info.plist
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