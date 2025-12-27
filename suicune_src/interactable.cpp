#include "interactable.hpp"

namespace suicune
{
    Interactable::Interactable(Scene *scene, int width, int height, float x, float y)
        : scene(scene), width(width), height(height), x(x), y(y)
    {
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

    void Interactable::update(int width, int height, float x, float y)
    {
        this->width = width;
        this->height = height;
        this->x = x;
        this->y = y;

        if (hoverable || clickable)
        {
            // Assuming `GetScreenToWorld2D`, `GetMousePosition`, `CheckCollisionPointRec`, and `IsMouseButtonPressed`
            // are functions provided by an external library, e.g., Raylib.
            Vector2 mouse_pos_world = GetScreenToWorld2D(GetMousePosition(), scene->get_camera());
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
}