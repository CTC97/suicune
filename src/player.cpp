#include "player.hpp"

namespace barley
{
    Player::Player(int x, int y) : x(x), y(y)
    {
    }

    Player::~Player()
    {
    }

    void Player::update(const Tilemap &tilemap)
    {
        int new_x = x;
        int new_y = y;

        const int speed = 1;
        if (IsKeyDown(KEY_RIGHT))
            new_x += speed;
        else if (IsKeyDown(KEY_LEFT))
            new_x -= speed;
        else if (IsKeyDown(KEY_UP))
            new_y -= speed;
        else if (IsKeyDown(KEY_DOWN))
            new_y += speed;

        int tileSize = tilemap.get_tile_size();

        int left = new_x;
        int right = new_x + 16 - 1;
        int top = new_y;
        int bottom = new_y + 16 - 1;

        int map_pixel_width = tilemap.get_map_width() * tileSize;
        int map_pixel_height = tilemap.get_map_height() * tileSize;

        auto is_free_pixel = [&](int px, int py)
        {
            // If the pixel is *outside* the map bounds, decide behavior.
            // Option A: allow leaving the map
            if (px < 0 || py < 0 || px >= map_pixel_width || py >= map_pixel_height)
                return false; // treat outside as not free

            // Now safe to convert to tile coords
            int tx = px / tileSize;
            int ty = py / tileSize;
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

    void Player::draw()
    {
        // Draw the player (e.g., use raylib to draw the player sprite)
        DrawRectangle(x, y, 16, 16, RED); // Example: Draw a red square as the player
    }

    Vector2 Player::get_position()
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }
}