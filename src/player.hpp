#pragma once

#include <string>
#include "raylib.h"
#include "spritesheet.hpp"

namespace barley
{
    class Player
    {

    public:
        Player(int x, int y);
        ~Player();

        void update();
        void draw();

        Vector2 get_position();

    private:
        int x;
        int y;
    };
}
