#pragma once

#include "../suicune_src/play_scene.hpp"

namespace suicune
{

    class TownScene : public PlayScene
    {
    public:
        TownScene(Game &game);
        ~TownScene() override;

        void draw() override;
        void update(float dt) override;

    protected:
        Camera2D &get_camera() override;

    private:
        Camera2D camera{};
        std::unique_ptr<Spritesheet> tilemap_spritesheet;
        std::unique_ptr<Spritesheet> nurse_joy_spritesheet;
        std::unique_ptr<Spritesheet> old_man_spritesheet;
        std::unique_ptr<Spritesheet> player_spritesheet;
        std::unique_ptr<Spritesheet> pokeball_spritesheet;
    };

}