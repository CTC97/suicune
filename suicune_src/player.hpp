#pragma once

#include <memory>
#include <vector>

#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "entity.hpp"

namespace suicune
{
    class Player : public Entity
    {
    public:
        Player(std::shared_ptr<Spritesheet> spritesheet, int width, int height, int x, int y);
        ~Player() override = default;

        // suppresses hidden override warning
        using Entity::update;

        // put this at the Entity level if we want moving entities later --> more likely to be a Mob extension of Entity
        void update(float dt, const Tilemap &tilemap, const std::vector<BoundBox> &entity_bound_boxes);

        void set_stop_movement(bool stop);
        bool is_movement_stopped() const;

    private:
        bool stop_movement;
    };
}
