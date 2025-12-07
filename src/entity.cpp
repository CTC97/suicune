#include "entity.hpp"

namespace barley
{
    Entity::Entity(Spritesheet &spritesheet, /*int width, int height,*/ int x, int y)
        : spritesheet(spritesheet), /*width(width), height(height),*/ x(x), y(y)
    {
    }

    void Entity::update()
    {
    }

    void Entity::draw()
    {
        spritesheet.draw_sprite(0, x, y);
        // DrawRectangle(x, y, 16, 16, RED);
    }

    Vector2 Entity::get_position() const
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    void Entity::set_solid(bool solid)
    {
        this->solid = solid;
    }

    bool Entity::is_solid() const
    {
        return solid;
    }
}