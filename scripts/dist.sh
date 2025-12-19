#!/usr/bin/env bash
set -euo pipefail

make

# ---- config ----
DIST_NAME="${1:-${DIST_NAME:-dist}}"  # Use the first argument, or environment variable, or default\
BIN_PATH="build/app"  # hardcoded binary path
RES_DIR="${RES_DIR:-res}"              # your resources folder
DIST_DIR="${DIST_DIR:-dist}"           # output folder

APP_DIR="$DIST_DIR/$DIST_NAME.app"
MACOS_DIR="$APP_DIR/Contents/MacOS"
RSRC_DIR="$APP_DIR/Contents/Resources"

# ---- checks ----
if [[ ! -f "$BIN_PATH" ]]; then
  echo "error: binary not found at '$BIN_PATH' (build it first)" >&2
  exit 1
fi

if [[ ! -d "$RES_DIR" ]]; then
  echo "error: resources folder not found at '$RES_DIR'" >&2
  exit 1
fi

# ---- create bundle ----
rm -rf "$APP_DIR"
mkdir -p "$MACOS_DIR" "$RSRC_DIR"

# Copy binary into the bundle (named exactly CFBundleExecutable)
cp "$BIN_PATH" "$MACOS_DIR/$DIST_NAME"
chmod +x "$MACOS_DIR/$DIST_NAME"

# Copy resources into Contents/Resources/res
cp -R "$RES_DIR" "$RSRC_DIR/$RES_DIR"

# Minimal Info.plist
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

echo ".app created:"
echo " .. $APP_DIR"