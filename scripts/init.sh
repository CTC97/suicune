#!/usr/bin/env bash
set -euo pipefail

# Define colors
RESET="\033[0m"
MEDIUM_BLUE="\033[38;5;24m"  # #4A6FA5
LIGHT_BLUE="\033[38;5;153m"  # #E6EEF7
PURPLE="\033[38;5;139m"  # #B7AEC8
LIGHT_PURPLE="\033[38;5;140m"  # #9E97B8


# Usage: ./barley-create.sh <project_name>
# Example: ./barley-create.sh project

REPO_URL="https://github.com/CTC97/barley"      # placeholder for now
SRC_FOLDER_IN_REPO="barley_src"

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
tmp_dir=".barley_repo_tmp"
git clone -q "$REPO_URL" "$tmp_dir"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"

echo -e "${MEDIUM_BLUE}Extracting source code...${RESET}"
if [[ ! -d "$tmp_dir/$SRC_FOLDER_IN_REPO" ]]; then
    echo -e "${LIGHT_PURPLE}Error: '$SRC_FOLDER_IN_REPO' not found in repo.${RESET}"
    echo -e "${LIGHT_PURPLE}Looked for: $tmp_dir/$SRC_FOLDER_IN_REPO${RESET}"
    rm -rf "$tmp_dir"
    exit 1
fi

mv "$tmp_dir/$SRC_FOLDER_IN_REPO" "./barley_src"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"

echo -e "${MEDIUM_BLUE}Cleaning up...${RESET}"
rm -rf "$tmp_dir"
echo -e "\t${LIGHT_BLUE}Done.${RESET}\n"

echo -e "${MEDIUM_BLUE}Creating project files...${RESET}"
echo -e "\t\t${PURPLE}Makefile${RESET}"
cat > Makefile <<'MAKEFILE'
# ---- basic config ----
CXX = g++
CXXFLAGS = -std=c++17 -Wall

barley_src = $(wildcard *.cpp) $(wildcard ../barley_src/*.cpp)
OUT = build

# macOS defaults
RAY_INC = /opt/homebrew/include
RAY_LIB = /opt/homebrew/lib

# Windows defaults
# Uncomment and adjust the paths below if building on Windows
# RAY_INC = C:/raylib/include
# RAY_LIB = C:/raylib/lib

# Linux defaults
# Uncomment and adjust the paths below if building on Linux
# RAY_INC = /usr/include
# RAY_LIB = /usr/lib

LIBS = -L$(RAY_LIB) -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all:
	$(CXX) $(CXXFLAGS) -I$(RAY_INC) $(barley_src) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)

MAKEFILE



echo -e "\t\t${PURPLE}main.cpp${RESET}"
cat > main.cpp <<'CPP'
#include "barley_src/game.hpp"
#include "src/main_scene.hpp"

using namespace barley;

int main()
{
    Game game("Barley Test", 1200, 700, 16);

    game.set_scene(std::make_unique<MainScene>(game));

    game.run();
}
CPP

echo -e "\t\t${PURPLE}res/, src/${RESET}"
mkdir -p res src

echo -e "\t\t\t${PURPLE}src/main_scene.hpp${RESET}"
cat > src/main_scene.hpp <<'CPP'
#pragma once

#include "../barley_src/play_scene.hpp"

namespace barley
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
#include "../barley_src/game.hpp"

namespace barley
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
