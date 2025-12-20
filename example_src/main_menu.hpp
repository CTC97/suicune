#pragma once

#include "../suicune_src/menu_scene.hpp"
#include "../suicune_src/game.hpp"

namespace suicune
{

    class MainMenu : public MenuScene
    {
    public:
        MainMenu(Game &game);
        ~MainMenu() override;

        void draw() override;
        using MenuScene::update;

    private:
        Texture2D play_texture;
    };

}