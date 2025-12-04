#pragma once

#include "scene.hpp"

namespace barley
{

    class PlayScene : public Scene
    {
    public:
        explicit PlayScene(Game &game);
        ~PlayScene() override;

        void update(float dt) override;
        void draw() override;
    };

}