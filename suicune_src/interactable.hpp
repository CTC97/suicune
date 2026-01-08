#pragma once

#include <functional>
#include "raylib.h"
#include "scene.hpp"

namespace suicune
{
    class Interactable
    {
    public:
        Interactable(Scene *scene, int width, int height, float x, float y);

        void update(float dt, int width, int height, float x, float y);

        float get_x();
        float get_y();

        void set_clickable(bool clickable);
        bool is_clickable() const;
        void set_clicked_callback(std::function<void()> callback);

        void set_hoverable(bool hoverable);
        bool is_hoverable() const;
        void set_hovered_callback(std::function<void()> callback);
        bool is_hovered_over() const;
        void set_unhovered_callback(std::function<void()> callback);

        void tween_to(Vector2 target, float duration);
        void tween_to(Vector2 target, float duration, std::function<void()> on_finished);
        bool is_tweening() const;
        void cancel_tween();
        bool does_tween_stop_movement();

    private:
        Scene *scene;
        int width;
        int height;
        float x;
        float y;

        std::function<void()> clicked_callback;
        std::function<void()> hovered_callback;
        std::function<void()> unhovered_callback;

        bool clickable = false;
        bool hoverable = false;
        bool hovered_over = false;

        Tween tween;
    };
}