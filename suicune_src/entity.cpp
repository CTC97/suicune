#include "entity.hpp"

namespace suicune
{
    Entity::Entity(Spritesheet &spritesheet, int width, int height, int x, int y, bool solid)
        : spritesheet(spritesheet), width(width), height(height), x(x), y(y), solid(solid)
    {
        set_bound_dimensions(width, height);
    }

    void Entity::update(float dt)
    {
        spritesheet.update_animation(dt);

        bound_box.x = x + (width - bound_box.width) / 2;
        bound_box.y = y + (height - bound_box.height) / 2;
    }

    void Entity::draw()
    {
        printf("Entity Position: x=%d, y=%d\n", x, y);
        spritesheet.draw_current_frame(static_cast<float>(x), static_cast<float>(y));
        // Draw the bounding box as a rectangle
        printf("Entity BoundBox: x=%d, y=%d, w=%d, h=%d\n", bound_box.x, bound_box.y, bound_box.width, bound_box.height);
        DrawRectangleLines(bound_box.x, bound_box.y, bound_box.width, bound_box.height, {255, 0, 0, 255}); // Red color
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

    void Entity::set_bound_dimensions(int bound_width, int bound_height)
    {
        bound_box.width = bound_width;
        bound_box.height = bound_height;
        bound_box.x = x + (width - bound_width) / 2;
        bound_box.y = y + (height - bound_height) / 2;
    }

    BoundBox Entity::get_bound_box() const
    {
        return bound_box;
    }

    Direction Entity::get_current_direction()
    {
        return current_direction;
    }
}