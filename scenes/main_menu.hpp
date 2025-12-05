#pragma once

#include "../src/menu_scene.hpp"

namespace barley
{

    class MainMenu : public MenuScene
    {
    public:
        MainMenu(Game &game);
        ~MainMenu() override;

        void draw() override;
        using MenuScene::update;

    private:
        Texture2D bg_texture;
    };

}