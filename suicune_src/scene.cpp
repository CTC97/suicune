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
        unload_spritesheets();
        unload_shaders();
    }

    Game &Scene::get_game()
    {
        return game;
    }

    void Scene::request_scene(std::unique_ptr<Scene> new_scene)
    {
        transitioning_scene = true;
        game.request_scene(std::move(new_scene));
    }

    void Scene::update(float dt)
    {
        if (transitioning_scene)
            return;

        if (screen_shake.timer > 0.0f)
        {
            screen_shake.timer -= dt;
            if (screen_shake.timer < 0.0f)
                screen_shake.is_alive = false;
        }

        if (screen_shake.is_alive)
        {
            float shake_x = ((float)GetRandomValue(-100, 100) / 100.0f) * screen_shake.strength;
            float shake_y = ((float)GetRandomValue(-100, 100) / 100.0f) * screen_shake.strength;
            camera.target.x += shake_x;
            camera.target.y += shake_y;
        }
    }

    void Scene::setup_draw()
    {
        Camera2D &camera = get_camera();
        BeginMode2D(camera);

        if (scene_shader)
        {
            float t = GetTime();
            int timeLoc = GetShaderLocation(*scene_shader, "time");
            SetShaderValue(*scene_shader, timeLoc, &t, SHADER_UNIFORM_FLOAT);
            Vector2 res = {(float)game.get_window_width(), (float)game.get_window_height()};
            int resLoc = GetShaderLocation(*scene_shader, "resolution");
            SetShaderValue(*scene_shader, resLoc, &res, SHADER_UNIFORM_VEC2);
            BeginShaderMode(*scene_shader);
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

        if (scene_shader)
        {
            EndShaderMode();
        }
    }

    void Scene::cleanup_draw()
    {
        EndMode2D();

        if (scene_shader)
        {
            EndShaderMode();
        }
    }

    std::shared_ptr<Spritesheet> Scene::define_spritesheet(const char *file_path, int frame_width, int frame_height)
    {
        spritesheets[file_path] = std::make_shared<Spritesheet>(
            file_path,
            frame_width,
            frame_height);

        return spritesheets[file_path];
    }

    void Scene::unload_spritesheets()
    {
        for (auto &pair : spritesheets)
        {
            UnloadTexture(pair.second->get_texture());
        }
        spritesheets.clear();
    }

    Camera2D &Scene::get_camera()
    {
        return camera;
    }

    void Scene::shake(float strength, float duration)
    {
        screen_shake.is_alive = true;
        screen_shake.strength = strength;
        screen_shake.timer = duration;
    }

    std::shared_ptr<Shader> Scene::define_shader(const char *file_path)
    {
        shaders[file_path] = std::make_shared<Shader>(LoadShader(0, TextFormat(file_path, GLSL_VERSION)));
        return shaders[file_path];
    }

    void Scene::set_scene_shader(std::shared_ptr<Shader> shader)
    {
        scene_shader = shader;
    }

    void Scene::clear_scene_shader()
    {
        scene_shader.reset();
    }

    void Scene::unload_shaders()
    {
        for (auto &pair : shaders)
        {
            UnloadShader(*pair.second);
        }
        shaders.clear();
    }
}