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

        virtual void update(float dt) = 0;
        virtual void draw();

    protected:
        Game &game;
    };

}
