#pragma once

#include "../barley_src/menu_scene.hpp"
#include "../barley_src/game.hpp"

namespace barley
{

    class MainMenu : public MenuScene
    {
    public:
        MainMenu(Game &game);
        ~MainMenu() override;

        void draw() override;
        using MenuScene::update;

        // private:
        //     Texture2D bg_texture;
    };

}