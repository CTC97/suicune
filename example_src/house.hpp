#pragma once

#include <memory>

#include "../suicune_src/entity.hpp"
#include "../suicune_src/spritesheet.hpp"
#include "main_menu.hpp"

namespace suicune
{
    class House : public Entity
    {
    public:
        House(Scene *scene, std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y);
    };
}
