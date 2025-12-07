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

        virtual void update();
        void draw();

        Vector2 get_position();

        void set_solid(bool solid);
        bool is_solid() const;

    private:
        Spritesheet &spritesheet;
        // int width;
        // int height;
        bool solid = true;
        int x;
        int y;
    };
}