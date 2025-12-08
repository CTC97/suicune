#pragma once

#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "entity.hpp"

namespace barley
{
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Player
    {

    public:
        Player(Spritesheet &spritesheet, int x, int y);
        ~Player();

        void update(const Tilemap &tilemap, const std::vector<std::vector<bool>> &entity_collision_map);
        void draw();

        Vector2 get_position();

        Direction get_current_direction();

    private:
        Spritesheet &spritesheet;

        int x;
        int y;

        Direction current_direction;
    };
}
