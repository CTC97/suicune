#pragma once

#include <memory>

#include "../suicune_src/entity.hpp"
#include "../suicune_src/spritesheet.hpp"

namespace suicune
{
    class Tree : public Entity
    {
    public:
        Tree(std::shared_ptr<Spritesheet> spritesheet, int width, int height, int x, int y);

    private:
        bool inverted = false;

        void interact() override;
    };
}
