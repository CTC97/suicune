#pragma once

#include "raylib.h"

namespace suicune
{

    class Game;

    class Scene
    {
    public:
        explicit Scene(Game &game);
        virtual ~Scene();

        virtual void update(float dt);
        virtual void draw();

        virtual void cleanup_eoframe() = 0;

    protected:
        Game &game;
        bool transitioning_scene = false;
    };

}
