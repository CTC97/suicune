#include "entity.hpp"

namespace suicune
{
    Entity::Entity(std::shared_ptr<Spritesheet> spritesheet, int width, int height, int x, int y)
        : animator(std::move(spritesheet)), width(width), height(height), x(x), y(y)
    {
        set_bound_box_position(x, y);
        set_bound_box_dimensions(width, height);
        set_bound_box_offset((width - bound_box.width) / 2, (height - bound_box.height) / 2);
    }

    void Entity::update(float dt)
    {
        animator.update(dt);
        set_bound_box_position(x + bound_box.offset_x, y + bound_box.offset_y);
    }

    void Entity::draw()
    {
        animator.draw_current_frame(static_cast<float>(x), static_cast<float>(y));

        // bounding box debug draw
        DrawRectangleLines(bound_box.x, bound_box.y, bound_box.width, bound_box.height, {255, 0, 0, 255});
        DrawCircle(bound_box.x + bound_box.width / 2, bound_box.y + bound_box.height / 2, 0.5, {255, 255, 0, 255});
    }

    void Entity::play_animation(const std::string &animation_name)
    {
        animator.play_animation(animation_name);
    }

    void Entity::play_animation(const std::string &animation_name, std::function<void()> on_finished)
    {
        animator.play_animation(animation_name, std::move(on_finished));
    }

    std::string Entity::get_current_animation() const
    {
        return animator.get_current_animation();
    }

    Vector2 Entity::get_position() const
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    int Entity::get_width()
    {
        return width;
    }

    int Entity::get_height()
    {
        return height;
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
        if (interaction_callback)
        {
            interaction_callback();
        }
        else
        {
            if (dm && !dialog_sequence.empty())
            {
                dm->start_dialog(dialog_sequence);
            }
        }
    }

    void Entity::collide()
    {
        if (collision_callback)
        {
            collision_callback();
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

    void Entity::set_bound_box_position(int bound_x, int bound_y)
    {
        bound_box.x = bound_x;
        bound_box.y = bound_y;
    }

    // note: this function also recenters the bound box offset
    void Entity::set_bound_box_dimensions(int bound_width, int bound_height)
    {
        bound_box.width = bound_width;
        bound_box.height = bound_height;
        set_bound_box_offset((width - bound_box.width) / 2, (height - bound_box.height) / 2);
    }

    void Entity::set_bound_box_offset(int offset_x, int offset_y)
    {
        bound_box.offset_x = offset_x;
        bound_box.offset_y = offset_y;
    }

    BoundBox Entity::get_bound_box() const
    {
        return bound_box;
    }

    Direction Entity::get_current_direction()
    {
        return current_direction;
    }

    Spritesheet &Entity::get_spritesheet() const
    {
        return animator.get_spritesheet();
    }

    // maybe theres a use for these somewhere?
    void Entity::set_interaction_callback(std::function<void()> callback)
    {
        interaction_callback = callback;
    }

    void Entity::set_collision_callback(std::function<void()> callback)
    {
        collision_callback = callback;
    }
}
