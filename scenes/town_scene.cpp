#include "town_scene.hpp"

namespace barley
{

    TownScene::TownScene(Game &game)
        : PlayScene(game)
    {
    }

    TownScene::~TownScene() = default;

    void TownScene::draw()
    {
        PlayScene::draw();
    }
}
