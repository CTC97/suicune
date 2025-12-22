#pragma once

#include "raylib.h"
#include "util.hpp"

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

        virtual Camera2D &get_camera();

        void shake(float strength, float duration)
        {
            screen_shake.strength = strength;
            screen_shake.timer = duration;
        }

    protected:
        Game &game;
        Camera2D camera{};

        bool transitioning_scene = false;

        Shake screen_shake;
    };

}
