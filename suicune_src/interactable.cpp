#include "interactable.hpp"

namespace suicune
{
    Interactable::Interactable(Scene *scene, int width, int height, float x, float y)
        : scene(scene), width(width), height(height), x(x), y(y)
    {
    }

    float Interactable::get_x()
    {
        return x;
    }

    float Interactable::get_y()
    {
        return y;
    }

    void Interactable::set_clickable(bool clickable)
    {
        this->clickable = clickable;
    }

    bool Interactable::is_clickable() const
    {
        return clickable;
    }

    void Interactable::set_clicked_callback(std::function<void()> callback)
    {
        clicked_callback = callback;
    }

    void Interactable::set_hoverable(bool hoverable)
    {
        this->hoverable = hoverable;
    }

    bool Interactable::is_hoverable() const
    {
        return hoverable;
    }

    void Interactable::set_hovered_callback(std::function<void()> callback)
    {
        hovered_callback = callback;
    }

    bool Interactable::is_hovered_over() const
    {
        return hovered_over;
    }

    void Interactable::set_unhovered_callback(std::function<void()> callback)
    {
        unhovered_callback = callback;
    }

    void Interactable::update(float dt, int width, int height, float x, float y)
    {
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;

        if (tween.active)
        {
            Vector2 p;
            step_tween(tween, dt, p);

            this->x = p.x;
            this->y = p.y;
        }

        if (hoverable || clickable)
        {
            // Assuming `GetScreenToWorld2D`, `GetMousePosition`, `CheckCollisionPointRec`, and `IsMouseButtonPressed`
            // are functions provided by an external library, e.g., Raylib.
            Vector2 mouse_pos_world = GetScreenToWorld2D(GetMousePosition(), scene->get_camera().get_ray_camera());
            if (CheckCollisionPointRec(mouse_pos_world, {x, y, (float)width, (float)height}))
            {
                if (hoverable)
                {
                    if (!hovered_over && hovered_callback)
                        hovered_callback();
                    hovered_over = true;
                }

                if (clickable && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (clicked_callback)
                        clicked_callback();
                }
            }
            else if (hoverable)
            {
                if (hovered_over && unhovered_callback)
                    unhovered_callback();
                hovered_over = false;
            }
        }
    }

    void Interactable::tween_to(Vector2 target, float duration)
    {
        tween.active = true;
        tween.start = Vector2{(float)x, (float)y};
        tween.target = target;
        tween.duration = duration;
        tween.elapsed = 0.0f;
        tween.stop_movement = true;
    }

    void Interactable::tween_to(Vector2 target, float duration, std::function<void()> on_finished)
    {
        tween_to(target, duration);
        tween.on_finished = std::move(on_finished);
    }

    bool Interactable::is_tweening() const
    {
        return tween.active;
    }

    void Interactable::cancel_tween()
    {
        tween.active = false;
    }

    bool Interactable::does_tween_stop_movement()
    {
        return tween.stop_movement;
    }
}