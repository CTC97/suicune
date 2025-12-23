#pragma once

#include "../suicune_src/play_scene.hpp"
#include "tree.hpp"
#include "house.hpp"

namespace suicune
{

    class TownScene : public PlayScene
    {
    public:
        TownScene(Game &game);

        void draw() override;
        void update(float dt) override;

    protected:
        Camera2D &get_camera() override;

    private:
        Camera2D camera{};
    };

}