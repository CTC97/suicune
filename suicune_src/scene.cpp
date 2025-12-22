#include "scene.hpp"
#include "game.hpp"
#include <iostream>

namespace suicune
{

    Scene::Scene(Game &game) : game(game)
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }

    Scene::~Scene()
    {
    }

    void Scene::update(float dt)
    {
        if (transitioning_scene)
        {
            TraceLog(LOG_INFO, "TRANSITIONING SCENE!!! [Update]");
            return;
        }

        if (screen_shake.timer > 0.0f)
        {
            screen_shake.timer -= dt;
            if (screen_shake.timer < 0.0f)
                screen_shake.is_alive = false;
        }
    }

    void Scene::draw()
    {
        if (transitioning_scene)
        {
            TraceLog(LOG_INFO, "TRANSITIONING SCENE!!! [Draw]");
            return;
        }

        if (game.is_dev_mode())
            DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 8, 10, WHITE);
    }

    Camera2D &Scene::get_camera()
    {
        return camera;
    }
}