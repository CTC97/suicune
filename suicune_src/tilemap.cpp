#include "tilemap.hpp"
#include <stdexcept> // For std::invalid_argument

namespace suicune
{
    // Constructor
    Tilemap::Tilemap(Spritesheet &spritesheet, int tile_size, int map_width, int map_height, std::vector<std::vector<int>> map)
        : spritesheet(spritesheet), tile_size(tile_size), map_width(map_width), map_height(map_height), map(std::move(map))
    {
        // Validate the dimensions of the map
        if (this->map.size() != map_height || (map_height > 0 && this->map[0].size() != map_width))
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
                int tile_index = map[y][x];
                spritesheet.draw_sprite(tile_index, x * tile_size, y * tile_size);
            }
        }
    }

    void Tilemap::set_collision_tiles(const std::vector<int> &tiles)
    {
        collision_tiles = tiles;
    }

    bool Tilemap::is_tile_free(int x, int y) const
    {
        if (x < 0 || x >= map_width || y < 0 || y >= map_height)
        {
            return false;
        }

        int tile_index = map[y][x];
        return std::find(collision_tiles.begin(), collision_tiles.end(), tile_index) == collision_tiles.end();
    }

    int Tilemap::get_tile_size() const
    {
        return tile_size;
    }

    int Tilemap::get_map_width() const
    {
        return map_width;
    }

    int Tilemap::get_map_height() const
    {
        return map_height;
    }
}