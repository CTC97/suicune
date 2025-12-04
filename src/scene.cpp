#include "scene.hpp"
#include "game.hpp"

namespace barley
{

    Scene::Scene(Game &game) : game(game)
    {
    }

    Scene::~Scene() = default;

}