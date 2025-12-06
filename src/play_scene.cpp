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

    void PlayScene::set_tilemap(std::unique_ptr<Tilemap> map)
    {
        tilemap = std::move(map);
    }

    void PlayScene::draw()
    {
        Scene::draw();
        Camera2D &camera = get_camera();

        BeginMode2D(camera);

        if (tilemap)
        {
            tilemap->draw();
        }

        EndMode2D();
    }

}