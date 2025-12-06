#pragma once

#include "../src/play_scene.hpp"

namespace barley
{

    class TownScene : public PlayScene
    {
    public:
        TownScene(Game &game);
        ~TownScene() override;

        void draw() override;
        using PlayScene::update;

    protected:
        Camera2D &get_camera() override;

    private:
        Camera2D camera{};
        std::unique_ptr<Spritesheet> spritesheet;
    };

}