#include "entity.hpp"

namespace barley
{
    Entity::Entity(Spritesheet &spritesheet, /*int width, int height,*/ int x, int y)
        : spritesheet(spritesheet), /*width(width), height(height),*/ x(x), y(y)
    {
    }

    void Entity::update(float dt)
    {
        spritesheet.update_animation(dt);
    }

    void Entity::draw()
    {
        spritesheet.draw_current_frame(static_cast<float>(x), static_cast<float>(y));
    }

    void Entity::play_animation(const std::string &animation_name)
    {
        spritesheet.play_animation(animation_name);
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

    void Entity::interact()
    {
        printf("generic interaction...\n");
    }
}