#include "scene.hpp"
#include "game.hpp"
#include <iostream>

namespace suicune
{

    Scene::Scene(Game &game) : game(game)
    {
        suicune_min = LoadTexture("../suicune_src/res/sprites/suicune_min.png");
        if (suicune_min.id == 0)
        {
            std::cerr << "Failed to load texture: suicune_min" << std::endl;
        }
        else
        {
            std::cout << "Texture loaded successfully: suicune_min (ID: " << suicune_min.id << ")" << std::endl;
        }
    }

    Scene::~Scene()
    {
        UnloadTexture(suicune_min);
    }

    void Scene::draw()
    {
        DrawTexture(suicune_min, game.get_window_width() - 48, 4, WHITE);
        DrawText(TextFormat("FPS: %i", GetFPS()), game.get_window_width() - 48, 4 + suicune_min.height + 4, 10, WHITE);
    }

}