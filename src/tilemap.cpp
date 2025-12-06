#include "tilemap.hpp"
#include <stdexcept> // For std::invalid_argument

namespace barley
{
    // Constructor
    Tilemap::Tilemap(Spritesheet &spritesheet, int map_width, int map_height, std::vector<std::vector<int>> tile_indices)
        : spritesheet(spritesheet), map_width(map_width), map_height(map_height), tile_indices(std::move(tile_indices))
    {
        // Validate the dimensions of the tile_indices
        if (this->tile_indices.size() != map_height || (map_height > 0 && this->tile_indices[0].size() != map_width))
        {
            throw std::invalid_argument("Tile indices dimensions do not match the specified map width and height.");
        }
    }

    // Destructor
    Tilemap::~Tilemap()
    {
        // No manual cleanup needed since std::vector handles memory automatically
    }

    // Draw method
    void Tilemap::draw()
    {
        for (int y = 0; y < map_height; ++y)
        {
            for (int x = 0; x < map_width; ++x)
            {
                int tile_index = tile_indices[y][x];
                spritesheet.draw_sprite(tile_index, x, y);
            }
        }
    }
}