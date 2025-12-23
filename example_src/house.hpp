#pragma once

#include <memory>

#include "../suicune_src/entity.hpp"
#include "../suicune_src/spritesheet.hpp"

namespace suicune
{
    class House : public Entity
    {
    public:
        House(std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y);
    };
}
