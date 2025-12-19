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

    void Scene::draw()
    {
        // change display of things like this and entity bound boxes to dev mode flag
        DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 4 + suicune_min.height + 4, 10, WHITE);
    }

}