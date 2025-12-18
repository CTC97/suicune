#pragma once

#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "entity.hpp"

namespace suicune
{
    // maybe this should go at entity level?
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Player : public Entity
    {

    public:
        Player(Spritesheet &spritesheet, int width, int height, int x, int y);
        ~Player();

        // suppresses hidden override warning
        using Entity::update;

        // put this at the Entity level if we want moving entities later --> more likely to be a Mob extension of Entity
        void update(float dt, const Tilemap &tilemap, const std::vector<BoundBox> &entity_bound_boxes);

        Direction get_current_direction();

    private:
        Direction current_direction;
    };
}
