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

        // private:
        //     Texture2D bg_texture;
    };

}