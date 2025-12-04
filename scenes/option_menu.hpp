#pragma once

#include "../src/menu_scene.hpp"

namespace barley
{

    class OptionMenu : public MenuScene
    {
    public:
        OptionMenu(Game &game);
        ~OptionMenu() override;

        void update(float dt) override;
        void draw() override;
    };

}