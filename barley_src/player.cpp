#include "player.hpp"

namespace barley
{
    Player::Player(Spritesheet &spritesheet, int x, int y) : Entity(spritesheet, x, y)
    {
        current_direction = DOWN;
    }

    Player::~Player()
    {
    }

    void Player::update(float dt, const Tilemap &tilemap, const std::vector<std::vector<bool>> &entity_collision_map)
    {
        int new_x = x;
        int new_y = y;

        const int speed = 1;
        if (IsKeyDown(KEY_RIGHT))
        {
            new_x += speed;
            current_direction = RIGHT;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            new_x -= speed;
            current_direction = LEFT;
        }
        else if (IsKeyDown(KEY_UP))
        {
            new_y -= speed;
            current_direction = UP;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            new_y += speed;
            current_direction = DOWN;
        }

        int tile_size = tilemap.get_tile_size();

        int left = new_x;
        int right = new_x + 16 - 1;
        int top = new_y;
        int bottom = new_y + 16 - 1;

        int map_pixel_width = tilemap.get_map_width() * tile_size;
        int map_pixel_height = tilemap.get_map_height() * tile_size;

        auto is_free_pixel = [&](int px, int py)
        {
            // If the pixel is *outside* the map bounds, decide behavior.
            // Option A: allow leaving the map
            if (px < 0 || py < 0 || px >= map_pixel_width || py >= map_pixel_height)
                return false; // treat outside as not free

            // Now safe to convert to tile coords
            int tx = px / tile_size;
            int ty = py / tile_size;
            return tilemap.is_tile_free(tx, ty) && !entity_collision_map[ty][tx];
        };

        bool can_move =
            is_free_pixel(left, top) &&
            is_free_pixel(right, top) &&
            is_free_pixel(left, bottom) &&
            is_free_pixel(right, bottom);

        if (can_move)
        {
            x = new_x;
            y = new_y;
        }

        Entity::update(dt);
    }

    Direction Player::get_current_direction()
    {
        return current_direction;
    }
}