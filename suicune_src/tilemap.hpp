#pragma once

#include <vector>
#include "spritesheet.hpp"

namespace suicune
{
    class Tilemap
    {
    public:
        Tilemap(Spritesheet &spritesheet, int tile_size, int map_width, int map_height, std::vector<std::vector<int>>);
        ~Tilemap();

        void draw();

        void set_collision_tiles(const std::vector<int> &tiles);

        bool is_tile_free(int x, int y) const;

        int get_tile_size() const;
        int get_map_width() const;
        int get_map_height() const;

        Spritesheet &get_spritesheet() const;

    private:
        Spritesheet &spritesheet;
        int tile_size;
        int map_width;
        int map_height;

        std::vector<std::vector<int>> map;

        std::vector<int> collision_tiles;
    };
}