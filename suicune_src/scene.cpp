#include "scene.hpp"
#include "game.hpp"
#include <iostream>

namespace suicune
{

    Scene::Scene(Game &game) : game(game)
    {
        auto &c = camera.get_ray_camera();
        c.zoom = 1.0f;
        c.offset = {game.get_window_width() / 2.0f, game.get_window_height() / 2.0f};

        camera.set_target({game.get_window_width() / 2.0f, game.get_window_height() / 2.0f});
    }

    Scene::~Scene()
    {
        printf("Unloading scene resources...\n");
        unload_textures();
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

        camera.update(dt);

        if (screen_shake.timer > 0.0f)
        {
            screen_shake.timer -= dt;
            if (screen_shake.timer < 0.0f)
                screen_shake.is_alive = false;
        }

        // if (screen_shake.is_alive)
        // {
        //     float shake_x = ((float)GetRandomValue(-100, 100) / 100.0f) * screen_shake.strength;
        //     float shake_y = ((float)GetRandomValue(-100, 100) / 100.0f) * screen_shake.strength;
        //     camera.get_ray_camera().target.x += shake_x;
        //     camera.y += shake_y;
        // }
        if (screen_shake.is_alive)
        {
            float shake_x = ((float)GetRandomValue(-100, 100) / 100.0f) * screen_shake.strength;
            float shake_y = ((float)GetRandomValue(-100, 100) / 100.0f) * screen_shake.strength;
            auto &rc = camera.get_ray_camera();
            rc.target.x += shake_x;
            rc.target.y += shake_y;
        }

        if (IsKeyPressed(KEY_LEFT_CONTROL))
        {
            game.set_dev_mode(!game.is_dev_mode());
        }
    }

    void Scene::setup_draw_world()
    {
        setup_scene_shader();
        BeginMode2D(camera.get_ray_camera());
        mode2d_active = true;
    }

    void Scene::setup_draw_screen()
    {
        mode2d_active = false;
        setup_scene_shader();
    }

    void Scene::setup_scene_shader()
    {
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

    void Scene::cleanup_draw()
    {
        if (mode2d_active)
        {
            EndMode2D();
            mode2d_active = false;
        }

        if (game.is_dev_mode())
            DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 8, 10, WHITE);

        if (scene_shader)
            EndShaderMode();
    }

    void Scene::draw()
    {
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

    SceneCamera &Scene::get_camera()
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

    std::shared_ptr<Texture2D> Scene::define_texture(const char *file_path)
    {
        textures[file_path] = std::make_shared<Texture2D>(LoadTexture(file_path));
        return textures[file_path];
    }

    void Scene::unload_textures()
    {
        for (auto &pair : textures)
        {
            UnloadTexture(*pair.second);
        }
        textures.clear();
    }
}