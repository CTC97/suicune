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
        // change display of things like this and entity bound boxes to dev mode flag
        DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 8, 10, WHITE);
    }

}