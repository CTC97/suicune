#include "scene.hpp"
#include "game.hpp"
#include <iostream>

namespace barley
{

    Scene::Scene(Game &game) : game(game)
    {
        barley_min = LoadTexture("src/res/sprites/barley_min.png");
        if (barley_min.id == 0)
        {
            std::cerr << "Failed to load texture: barley_min" << std::endl;
        }
        else
        {
            std::cout << "Texture loaded successfully: barley_min (ID: " << barley_min.id << ")" << std::endl;
        }
    }

    Scene::~Scene() = default;

    void Scene::draw()
    {
        DrawTexture(barley_min, 0, game.get_window_height() - 32, WHITE);
    }

}