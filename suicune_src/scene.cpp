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
        unload_spritesheets();
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

        if (game.is_dev_mode())
            DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 8, 10, WHITE);
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
}