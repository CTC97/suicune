#include "town_scene.hpp"
#include "../src/game.hpp"
#include "../entities/nurse_joy.hpp"

namespace barley
{

    TownScene::TownScene(Game &game)
        : PlayScene(game)
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 4.0f;

        tilemap_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/tilesheet.png",
            16,
            16);

        std::vector<std::vector<int>> map_data = {
            {0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2},
            {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0},
            {0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2},
            {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0},
            {0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2},
            {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0},
        };

        tilemap = std::make_unique<Tilemap>(*tilemap_spritesheet, 16, map_data[0].size(), map_data.size(), map_data);
        tilemap->set_collision_tiles({3});

        initialize_entity_collision_map();

        player_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/Sprite-0001.png",
            16,
            16);

        player = std::make_unique<Player>(*player_spritesheet, 16, 16);
        camera.target = player->get_position();

        nurse_joy_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/Sprite-0001.png",
            16,
            16);

        old_man_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/Sprite-0002.png",
            16,
            16);

        add_entity(std::make_unique<NurseJoy>(*nurse_joy_spritesheet, 0, 0));
        add_entity(std::make_unique<Entity>(*old_man_spritesheet, 64, 64));
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
