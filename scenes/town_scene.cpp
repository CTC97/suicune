#include "town_scene.hpp"

namespace barley
{

    TownScene::TownScene(Game &game)
        : PlayScene(game)
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 3.0f;

        auto sheet = std::make_unique<Spritesheet>(
            "res/sprites/tilesheet.png",
            16,
            16);

        // build your tilemap
        std::vector<std::vector<int>> map_data = {
            {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        };

        auto map = std::make_unique<Tilemap>(*sheet, map_data[0].size(), map_data.size(), map_data);

        // attach them
        // If PlayScene doesn't own the Spritesheet, TownScene must keep it alive
        spritesheet = std::move(sheet);
        set_tilemap(std::move(map));
    }

    TownScene::~TownScene() = default;

    void TownScene::draw()
    {
        PlayScene::draw();
    }

    Camera2D &TownScene::get_camera()
    {
        return camera;
    }
}
