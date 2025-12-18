#include "player.hpp"

namespace suicune
{
    Player::Player(Spritesheet &spritesheet, int width, int height, int x, int y) : Entity(spritesheet, width, height, x, y)
    {
        current_direction = DOWN;
    }

    Player::~Player()
    {
    }

    void Player::update(float dt, const Tilemap &tilemap, const std::vector<BoundBox> &entity_bound_boxes)
    {
        Entity::update(dt);

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

            // Compare player BoundBox to entity_bound_boxes:

            for (const auto &b : entity_bound_boxes)
            {
                if (check_bound_box_collision(bound_box, b, 10))
                    return false;
            }

            return tilemap.is_tile_free(tx, ty);
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
    }

    Direction Player::get_current_direction()
    {
        return current_direction;
    }
}