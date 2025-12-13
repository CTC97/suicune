// x, y, spritesheet, width, height
#pragma once

#include "raylib.h"
#include "spritesheet.hpp"

namespace barley
{
    class Entity
    {
    public:
        Entity(Spritesheet &spritesheet, /*int width, int height,*/ int x, int y);
        virtual ~Entity() = default;

        virtual void update(float dt);
        virtual void draw();

        void play_animation(const std::string &animation_name);
        std::string get_current_animation() const;

        Vector2 get_position() const;

        void set_solid(bool solid);
        bool is_solid() const;

        virtual void interact();

    protected:
        Spritesheet &spritesheet;
        // int width;
        // int height;
        bool solid = true;
        int x;
        int y;
    };
}