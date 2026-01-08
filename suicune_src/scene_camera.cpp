#include "scene_camera.hpp"

namespace suicune
{

    Camera2D &SceneCamera::get_ray_camera()
    {
        return camera;
    }

    void SceneCamera::update(float dt)
    {
        // if (!tween.active && x == 0.0f && y == 0.0f)
        // {
        //     x = camera.target.x;
        //     y = camera.target.y;
        // }

        if (tween.active)
        {
            Vector2 p;
            step_tween(tween, dt, p);
            x = p.x;
            y = p.y;
        }

        camera.target = {x, y};
    }

    void SceneCamera::set_target(Vector2 t)
    {
        x = t.x;
        y = t.y;
        camera.target = t;
    }

    Vector2 SceneCamera::get_target() const
    {
        return {x, y};
    }

    void SceneCamera::tween_to(Vector2 target, float duration)
    {
        tween.active = true;
        tween.start = Vector2{x, y};
        tween.target = target;
        tween.duration = duration;
        tween.elapsed = 0.0f;
        tween.stop_movement = true;
    }

    void SceneCamera::tween_to(Vector2 target, float duration, std::function<void()> on_finished)
    {
        tween_to(target, duration);
        tween.on_finished = std::move(on_finished);
    }

    bool SceneCamera::is_tweening() const
    {
        return tween.active;
    }

    void SceneCamera::cancel_tween()
    {
        tween.active = false;
    }

    bool SceneCamera::does_tween_stop_movement()
    {
        return tween.stop_movement;
    }
}