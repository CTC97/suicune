#pragma once

#include <vector>
#include "spritesheet.hpp"

namespace barley
{
    class Tilemap
    {
    public:
        Tilemap(Spritesheet &spritesheet, int map_width, int map_height, std::vector<std::vector<int>>);
        ~Tilemap();

        void draw();

    private:
        Spritesheet &spritesheet;
        int map_width;
        int map_height;
        std::vector<std::vector<int>> tile_indices;
    };
}