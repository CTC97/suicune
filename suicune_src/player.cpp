#include "player.hpp"

namespace suicune
{
    Player::Player(std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y)
        : Entity(std::move(spritesheet), width, height, x, y)
    {
        current_direction = DOWN;
    }

    void Player::update(float dt, const Tilemap &tilemap, const std::vector<std::unique_ptr<Entity>> &entities)
    {
        Entity::update(dt);

        if (stop_movement || (tween.active && tween.stop_movement))
            return;

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
            if (px < 0 || py < 0 || px >= map_pixel_width || py >= map_pixel_height)
                return false;

            int tx = px / tile_size;
            int ty = py / tile_size;

            return tilemap.is_tile_free(tx, ty);
        };

        bool can_move =
            is_free_pixel(left, top) &&
            is_free_pixel(right, top) &&
            is_free_pixel(left, bottom) &&
            is_free_pixel(right, bottom);

        BoundBox prev = get_bound_box();
        BoundBox next = prev;

        next.x += (new_x - x);
        next.y += (new_y - y);

        for (const auto &entity : entities)
        {

            if (check_directional_bound_box_collision(prev, next, entity->get_bound_box(), current_direction))
            {
                entity->collide();
                if (entity->is_solid())
                    can_move = false;
                break;
            }
        }

        if (can_move)
        {
            x = new_x;
            y = new_y;
        }
    }
}
