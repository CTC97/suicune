#include "town_scene.hpp"
#include "../suicune_src/game.hpp"

namespace suicune
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
            {0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5},
            {5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0},
            {0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5},
            {5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0},
            {0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5},
            {5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0},
        };

        tilemap = std::make_unique<Tilemap>(*tilemap_spritesheet, 16, map_data[0].size(), map_data.size(), map_data);
        tilemap->set_collision_tiles({3});

        // player
        player_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/bw_player.png",
            16,
            16);

        player_spritesheet->define_animation("still", {{0}, 1.0f, true});
        player_spritesheet->define_animation("walk_down", {{1, 2}, 0.2f, true});
        player_spritesheet->define_animation("walk_up", {{7, 8}, 0.2f, true});
        player_spritesheet->define_animation("walk_right", {{4, 5}, 0.2f, true});
        player_spritesheet->define_animation("walk_left", {{9, 10}, 0.2f, true});

        player = std::make_unique<Player>(*player_spritesheet, 16, 16, 16, 16);
        player->play_animation("still");
        player->set_bound_box_dimensions(8, 4);
        player->set_bound_box_offset(4, 12);
        camera.target = player->get_position();

        // nurse joy
        nurse_joy_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/Sprite-0001.png",
            16,
            16);
        nurse_joy_spritesheet->define_animation("still", {{0}, 1.0f, true});

        auto nurse_joy = std::make_unique<Entity>(*nurse_joy_spritesheet, 16, 16, 64, 64);
        nurse_joy->set_dialog({{"Welcome to the Pokecenter!", "Nurse Joy", {}}});
        nurse_joy->play_animation("still");
        add_entity(std::move(nurse_joy));

        // pokeball
        pokeball_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/pokeball.png",
            16,
            16);
        pokeball_spritesheet->define_animation("still", {{0}, 1.0f, true});
        auto pokeball = std::make_unique<Entity>(*pokeball_spritesheet, 16, 16, 100, 32);
        pokeball->set_bound_box_dimensions(8, 8);
        pokeball->play_animation("still");
        add_entity(std::move(pokeball));

        // tree
        tree_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/tree.png",
            16,
            32);
        tree_spritesheet->define_animation("still", {{0}, 1.0f, true});
        auto tree = std::make_unique<Entity>(*tree_spritesheet, 16, 32, 32, 32);
        tree->set_bound_box_dimensions(2, 2);
        tree->set_bound_box_offset(7, 30);
        tree->play_animation("still");
        add_entity(std::move(tree));
    }

    TownScene::~TownScene() = default;

    void TownScene::update(float dt)
    {
        if (player->get_current_direction() == RIGHT)
            player->play_animation("walk_right");
        else if (player->get_current_direction() == LEFT)
            player->play_animation("walk_left");
        else if (player->get_current_direction() == UP)
            player->play_animation("walk_up");
        else if (player->get_current_direction() == DOWN)
            player->play_animation("walk_down");
        // else
        //     player->play_animation("still");
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
