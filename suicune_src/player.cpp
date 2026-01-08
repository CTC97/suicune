#include "player.hpp"

namespace suicune
{
    Player::Player(Scene *scene, std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y)
        : Entity(scene, std::move(spritesheet), width, height, x, y)
    {
        current_direction = DOWN;
    }
    void Player::update(float dt, const Tilemap &tilemap, const std::vector<std::unique_ptr<Entity>> &entities)
    {
        Entity::update(dt);

        if (stop_movement || (is_tweening() && interactable.does_tween_stop_movement()))
            return;

        DIRECTIONAL_MODE directional_mode = scene->get_game().get_direction_mode();

        bool right_key_down = IsKeyDown(KEY_RIGHT);
        bool left_key_down = IsKeyDown(KEY_LEFT);
        bool up_key_down = IsKeyDown(KEY_UP);
        bool down_key_down = IsKeyDown(KEY_DOWN);

        // --- Determine intended movement (delta), but DON'T commit yet ---
        int step_x = 0;
        int step_y = 0;

        if (directional_mode == CARDINAL)
        {
            if (right_key_down)
            {
                step_x = +speed;
                current_direction = RIGHT;
            }
            else if (left_key_down)
            {
                step_x = -speed;
                current_direction = LEFT;
            }
            else if (up_key_down)
            {
                step_y = -speed;
                current_direction = UP;
            }
            else if (down_key_down)
            {
                step_y = +speed;
                current_direction = DOWN;
            }
        }
        else if (directional_mode == OCTAGONAL)
        {
            int dx = (right_key_down ? 1 : 0) - (left_key_down ? 1 : 0);
            int dy = (down_key_down ? 1 : 0) - (up_key_down ? 1 : 0);

            if (dx != 0 || dy != 0)
            {
                // normalize diagonal speed but keep ints
                float length = sqrtf((float)(dx * dx + dy * dy));
                float nx = dx / length;
                float ny = dy / length;

                step_x = (int)roundf(nx * speed);
                step_y = (int)roundf(ny * speed);

                // direction for animation/facing
                if (dx > 0 && dy < 0)
                    current_direction = UP_RIGHT;
                else if (dx > 0 && dy > 0)
                    current_direction = DOWN_RIGHT;
                else if (dx < 0 && dy < 0)
                    current_direction = UP_LEFT;
                else if (dx < 0 && dy > 0)
                    current_direction = DOWN_LEFT;
                else if (dx > 0)
                    current_direction = RIGHT;
                else if (dx < 0)
                    current_direction = LEFT;
                else if (dy < 0)
                    current_direction = UP;
                else
                    current_direction = DOWN;
            }
        }

        int tile_size = tilemap.get_tile_size();
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

        // checks the 4 corners for the player's 16x16 at a candidate position
        auto can_move_to = [&](int cx, int cy)
        {
            int left = cx;
            int right = cx + 16 - 1;
            int top = cy;
            int bottom = cy + 16 - 1;

            return is_free_pixel(left, top) &&
                   is_free_pixel(right, top) &&
                   is_free_pixel(left, bottom) &&
                   is_free_pixel(right, bottom);
        };

        // --- Axis-separated resolution (key fix) ---
        int candidate_x = x;
        int candidate_y = y;

        // 1) Try X move
        if (step_x != 0)
        {
            int try_x = candidate_x + step_x;
            bool can_x = can_move_to(try_x, candidate_y);

            BoundBox prev = get_bound_box();
            BoundBox next = prev;
            next.x += (try_x - x);
            next.y += (candidate_y - y);

            Direction x_dir = (step_x > 0) ? RIGHT : LEFT;

            for (const auto &entity : entities)
            {
                if (check_directional_bound_box_collision(prev, next, entity->get_bound_box(), x_dir))
                {
                    entity->collide();
                    if (entity->is_solid())
                        can_x = false;
                    break;
                }
            }

            if (can_x)
                candidate_x = try_x;
        }

        // 2) Try Y move (using updated candidate_x)
        if (step_y != 0)
        {
            int try_y = candidate_y + step_y;
            bool can_y = can_move_to(candidate_x, try_y);

            BoundBox prev = get_bound_box();
            BoundBox next = prev;
            next.x += (candidate_x - x);
            next.y += (try_y - y);

            Direction y_dir = (step_y > 0) ? DOWN : UP;

            for (const auto &entity : entities)
            {
                if (check_directional_bound_box_collision(prev, next, entity->get_bound_box(), y_dir))
                {
                    entity->collide();
                    if (entity->is_solid())
                        can_y = false;
                    break;
                }
            }

            if (can_y)
                candidate_y = try_y;
        }

        // Commit
        x = candidate_x;
        y = candidate_y;
    }
}
