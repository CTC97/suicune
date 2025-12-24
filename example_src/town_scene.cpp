#include "town_scene.hpp"
#include "../suicune_src/game.hpp"
#include "main_menu.hpp"
#include <memory>
#include "raymath.h" // Include the header for Vector2Add and other vector math functions

namespace suicune
{

    TownScene::TownScene(Game &game)
        : PlayScene(game)
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 4.0f;

        auto tilemap_spritesheet = define_spritesheet("res/sprites/tilesheet.png", 16, 16);
        std::vector<std::vector<int>>
            map_data = {
                {0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5},
                {5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0},
                {0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5},
                {5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0},
                {0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5},
                {5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0},
            };
        tilemap = std::make_unique<Tilemap>(*tilemap_spritesheet, 16, map_data[0].size(), map_data.size(), map_data);
        tilemap->set_collision_tiles({3});

        auto player_spritesheet = define_spritesheet("res/sprites/bw_player.png", 16, 16);
        player_spritesheet->define_animation("still", {{0}, 1.0f, true});
        player_spritesheet->define_animation("walk_down", {{1, 2}, 0.2f, true});
        player_spritesheet->define_animation("walk_up", {{7, 8}, 0.2f, true});
        player_spritesheet->define_animation("walk_right", {{4, 5}, 0.2f, true});
        player_spritesheet->define_animation("walk_left", {{9, 10}, 0.2f, true});
        player_spritesheet->define_animation("inv", {{0, 11, 0, 11, 0, 11, 0, 11}, 0.25f, false});

        // change this to a define syntax too
        player = std::make_unique<Player>(this, player_spritesheet, 16, 16, 16, 16);
        player->set_bound_box_dimensions(8, 4);
        player->set_bound_box_offset(4, 12);
        player->play_animation("still");
        camera.target = player->get_position();

        // tree spritesheet (shared asset)
        auto tree_spritesheet = define_spritesheet("res/sprites/tree.png", 16, 32);
        tree_spritesheet->define_animation("still", {{0}, 1.0f, true});
        tree_spritesheet->define_animation("inv", {{1}, 0.5f, true});

        for (int i = 0; i < 5; ++i)
        {
            define_entity<Tree>(tree_spritesheet, 16, 32, 32 + i * 32, 32);
        }

        auto house_spritesheet = define_spritesheet("res/sprites/house.png", 32, 32);
        house_spritesheet->define_animation("still", {{0}, 1.0f, true});
        define_entity<House>(house_spritesheet, 32, 32, 100, 50);
    }

    void TownScene::update(float dt)
    {
        PlayScene::update(dt);

        if (IsKeyPressed(KEY_I))
        {
            shake(1, 0.5);
        }

        if (player->is_movement_stopped())
            return;
        if (player->get_current_direction() == RIGHT)
            player->play_animation("walk_right");
        else if (player->get_current_direction() == LEFT)
            player->play_animation("walk_left");
        else if (player->get_current_direction() == UP)
            player->play_animation("walk_up");
        else if (player->get_current_direction() == DOWN)
            player->play_animation("walk_down");
        else
            player->play_animation("still");
    }

    void TownScene::draw()
    {
        PlayScene::draw();

        camera.target = Vector2Add(player->get_position(), Vector2{player->get_width() / 2.0f, player->get_height() / 2.0f});
        camera.offset = (Vector2){game.get_window_width() / 2.0f, game.get_window_height() / 2.0f};
    }

}
