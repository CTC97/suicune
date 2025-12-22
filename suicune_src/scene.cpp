#include "scene.hpp"
#include "game.hpp"
#include <iostream>

namespace suicune
{

    Scene::Scene(Game &game) : game(game)
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::update(float dt)
    {
        (void)dt;
        if (transitioning_scene)
        {
            TraceLog(LOG_INFO, "TRANSITIONING SCENE!!! [Update]");
            return;
        }
    }

    void Scene::draw()
    {
        if (transitioning_scene)
        {
            TraceLog(LOG_INFO, "TRANSITIONING SCENE!!! [Draw]");
            return;
        }

        if (shader)
        {
            TraceLog(LOG_INFO, "Drawing with shader");
            float t = GetTime();
            int timeLoc = GetShaderLocation(*shader, "time");
            SetShaderValue(*shader, timeLoc, &t, SHADER_UNIFORM_FLOAT);
            Vector2 res = {(float)game.get_window_width(), (float)game.get_window_height()};
            int resLoc = GetShaderLocation(*shader, "resolution");
            SetShaderValue(*shader, resLoc, &res, SHADER_UNIFORM_VEC2);
            BeginShaderMode(*shader);
        }

        if (game.is_dev_mode())
            DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 8, 10, WHITE);
    }

    void Scene::set_shader(Shader shader)
    {
        this->shader = std::make_shared<Shader>(shader);
    }

    void Scene::clear_shader()
    {
        shader.reset();
    }
}