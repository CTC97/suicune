#!/usr/bin/env bash
set -euo pipefail

# Define colors
RESET="\033[0m"
MEDIUM_BLUE="\033[38;5;24m"  # #4A6FA5
LIGHT_BLUE="\033[38;5;153m"  # #E6EEF7
PURPLE="\033[38;5;139m"  # #B7AEC8
LIGHT_PURPLE="\033[38;5;140m"  # #9E97B8


# Usage: ./suicune-create.sh <project_name>
# Example: ./suicune-create.sh project

REPO_URL="https://github.com/CTC97/suicune"      # placeholder for now
SRC_FOLDER_IN_REPO="suicune_src"

project_name="${1:-}"

if [[ -z "$project_name" ]]; then
    echo -e "${DARK_BLUE}Usage: $0 <project_name>${RESET}"
    exit 1
fi

if [[ -e "$project_name" ]]; then
    echo -e "${DARKER_BLUE}Error: '$project_name' already exists.${RESET}"
    exit 1
fi

echo -e "${MEDIUM_BLUE}Creating project ($project_name)${RESET}"
mkdir -p "$project_name"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"
cd "$project_name"

echo -e "${PURPLE}Cloning source code...${RESET}"
tmp_dir=".suicune_repo_tmp"
git clone -q "$REPO_URL" "$tmp_dir"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"

echo -e "${MEDIUM_BLUE}Extracting source code...${RESET}"
if [[ ! -d "$tmp_dir/$SRC_FOLDER_IN_REPO" ]]; then
    echo -e "${LIGHT_PURPLE}Error: '$SRC_FOLDER_IN_REPO' not found in repo.${RESET}"
    echo -e "${LIGHT_PURPLE}Looked for: $tmp_dir/$SRC_FOLDER_IN_REPO${RESET}"
    rm -rf "$tmp_dir"
    exit 1
fi

mv "$tmp_dir/$SRC_FOLDER_IN_REPO" "./suicune_src"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"

echo -e "${MEDIUM_BLUE}Cleaning up...${RESET}"
rm -rf "$tmp_dir"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"

echo -e "${MEDIUM_BLUE}Creating project files...${RESET}"
echo -e "\t\t${PURPLE}Makefile${RESET}"
cat > Makefile <<'MAKEFILE'
# -------- project config --------
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra

TARGET   ?= build/suicune

# Source layout (edit these globs if you move folders)
SRC := $(wildcard *.cpp) \
       $(wildcard ./suicune_src/*.cpp) \
       $(wildcard ./src/*.cpp)

OBJDIR := build/obj
OBJ := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

# -------- platform detection --------
UNAME_S := $(shell uname -s)

# Prefer pkg-config when available (most portable)
PKG_CONFIG ?= pkg-config
HAVE_PKGCONFIG := $(shell command -v $(PKG_CONFIG) >/dev/null 2>&1 && echo yes || echo no)

# Defaults (can be overridden: make RAY_INC=... RAY_LIB=...)
RAY_INC ?=
RAY_LIB ?=

ifeq ($(HAVE_PKGCONFIG),yes)
  # If raylib is installed with a .pc file, this is the cleanest path.
  RAY_CFLAGS := $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
  RAY_LIBS   := $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)
endif

# Fallbacks when pkg-config doesn't know raylib
ifeq ($(strip $(RAY_LIBS)),)
  ifeq ($(UNAME_S),Darwin)
    # Homebrew paths: Apple Silicon is /opt/homebrew, Intel is /usr/local
    ifeq ($(wildcard /opt/homebrew/include),/opt/homebrew/include)
      RAY_INC ?= /opt/homebrew/include
      RAY_LIB ?= /opt/homebrew/lib
    else
      RAY_INC ?= /usr/local/include
      RAY_LIB ?= /usr/local/lib
    endif

    RAY_CFLAGS += -I$(RAY_INC)
    RAY_LIBS   += -L$(RAY_LIB) -lraylib \
                  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
  else
    # Linux fallback (varies by distro, but this is common)
    RAY_INC ?= /usr/include
    RAY_LIB ?= /usr/lib

    RAY_CFLAGS += -I$(RAY_INC)
    RAY_LIBS   += -L$(RAY_LIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
  endif
endif

CPPFLAGS += $(RAY_CFLAGS)
LDFLAGS  += $(RAY_LIBS)

# -------- rules --------
.PHONY: all clean run print-config

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Compile .cpp -> .o into build/obj/ preserving subfolders
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf build

run: all
	./$(TARGET)

print-config:
	@echo "UNAME_S     = $(UNAME_S)"
	@echo "HAVE_PKGCFG = $(HAVE_PKGCONFIG)"
	@echo "CPPFLAGS    = $(CPPFLAGS)"
	@echo "LDFLAGS     = $(LDFLAGS)"
	@echo "SRC         = $(SRC)"

MAKEFILE



echo -e "\t\t${PURPLE}main.cpp${RESET}"
cat > main.cpp <<'CPP'
#include "suicune_src/game.hpp"
#include "src/main_scene.hpp"

using namespace suicune;

int main()
{
    Game game("suicune Test", 1200, 700, 16);

    game.set_scene(std::make_unique<MainScene>(game));

    game.run();
}
CPP

echo -e "\t\t${PURPLE}res/, src/${RESET}"
mkdir -p res src

echo -e "\t\t\t${PURPLE}src/main_scene.hpp${RESET}"
cat > src/main_scene.hpp <<'CPP'
#pragma once

#include "../suicune_src/play_scene.hpp"

namespace suicune
{
    class MainScene : public PlayScene
    {
    public:
        MainScene(Game &game);
        ~MainScene() override;

        void draw() override;
        void update(float dt) override;

    protected:
        Camera2D &get_camera() override;

    private:
        Camera2D camera{};
    };
}
CPP

echo -e "\t\t\t${PURPLE}src/main_scene.cpp${RESET}"
cat > src/main_scene.cpp <<'CPP'
#include "main_scene.hpp"
#include "../suicune_src/game.hpp"

namespace suicune
{

    MainScene::MainScene(Game &game)
        : PlayScene(game)
    {
    }

    MainScene::~MainScene() = default;

    void MainScene::update(float dt)
    {
    }

    void MainScene::draw()
    {
        PlayScene::draw();
    }

    Camera2D &MainScene::get_camera()
    {
        return camera;
    }
}
CPP


echo -e "\t${LIGHT_BLUE}Done${RESET}\n"

echo -e "${MEDIUM_BLUE}Have fun!${RESET}"
