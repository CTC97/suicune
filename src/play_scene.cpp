#include "play_scene.hpp"
#include "game.hpp"
#include "raylib.h"

namespace barley
{

    PlayScene::PlayScene(Game &game)
        : Scene(game)
    {
    }

    PlayScene::~PlayScene() = default;

    void PlayScene::update(float dt)
    {
        (void)dt;
        // game logic will go here later
    }

    void PlayScene::draw()
    {
        DrawText("PlayScene", 50, 50, 20, BLACK);
    }

}