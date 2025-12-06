#include "player.hpp"

namespace barley
{
    Player::Player(int x, int y) : x(x), y(y)
    {
    }

    Player::~Player()
    {
    }

    void Player::update()
    {
        if (IsKeyDown(KEY_RIGHT))
            x += 2;
        if (IsKeyDown(KEY_LEFT))
            x -= 2;
        if (IsKeyDown(KEY_UP))
            y -= 2;
        if (IsKeyDown(KEY_DOWN))
            y += 2;
    }

    void Player::draw()
    {
        // Draw the player (e.g., use raylib to draw the player sprite)
        DrawRectangle(x, y, 32, 32, RED); // Example: Draw a red square as the player
    }

    Vector2 Player::get_position()
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }
}