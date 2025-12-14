#include "entity.hpp"

namespace suicune
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

    std::string Entity::get_current_animation() const
    {
        return spritesheet.get_current_animation();
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
        if (dm && !dialog_sequence.empty())
        {
            dm->start_dialog(dialog_sequence);
        }
    }

    void Entity::set_dialog_manager(DialogManager *dialog_manager)
    {
        dm = dialog_manager;
    }

    void Entity::set_dialog(const std::vector<DialogNode> &dialog)
    {
        dialog_sequence = dialog;
    }
}