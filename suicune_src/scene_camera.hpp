#pragma once

#include "raylib.h"
#include "util.hpp"

namespace suicune
{

    class SceneCamera
    {
    public:
        float x;
        float y;

        Camera2D &get_ray_camera();

        void update(float dt);

        void set_target(Vector2 t);
        Vector2 get_target() const;

        void tween_to(Vector2 target, float duration);
        void tween_to(Vector2 target, float duration, std::function<void()> on_finished);
        bool is_tweening() const;
        void cancel_tween();
        bool does_tween_stop_movement();

    private:
        Camera2D camera;

        Tween tween;
    };
}