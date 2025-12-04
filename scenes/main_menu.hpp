#pragma once

#include "../src/menu_scene.hpp"

namespace barley
{

    class MainMenu : public MenuScene
    {
    public:
        MainMenu(Game &game);
        ~MainMenu() override;

        void update(float dt) override;
        void draw() override;
    };

}