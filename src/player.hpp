#pragma once

#include <string>
#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"

namespace barley
{
    class Player
    {

    public:
        Player(int x, int y);
        ~Player();

        void update(const Tilemap &tilemap);
        void draw();

        Vector2 get_position();

    private:
        int x;
        int y;
    };
}
