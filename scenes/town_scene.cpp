#include "town_scene.hpp"
#include "../src/game.hpp"

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

        // Attach + if PlayScene doesn't own the Spritesheet, TownScene must keep it alive
        spritesheet = std::move(sheet);
        set_tilemap(std::move(map));

        player = std::make_unique<Player>(16, 16);
        camera.target = player->get_position();

        /*

        I really want to make it so that we do something like:

            Spritesheet tile_sheet(...);
            Tilemap tilemap(tile_sheet, ...);
            set_tilemap(&tilemap);

            Spritesheet player_sheet(...);
            Player player(player_sheet, 5, 5, ...);
            set_player(&player);

        */
    }

    TownScene::~TownScene() = default;

    void TownScene::update(float dt)
    {
        PlayScene::update(dt);
    }

    void TownScene::draw()
    {
        camera.target = player->get_position();
        camera.offset = (Vector2){game.get_window_width() / 2.0f, game.get_window_height() / 2.0f};
        PlayScene::draw();
    }

    Camera2D &TownScene::get_camera()
    {
        return camera;
    }
}
