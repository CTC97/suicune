#include "town_scene.hpp"
#include "../suicune_src/game.hpp"

#include <memory>

namespace suicune
{

    TownScene::TownScene(Game &game)
        : PlayScene(game)
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 4.0f;

        // tilemap spritesheet (shared asset)
        tilemap_spritesheet = std::make_shared<Spritesheet>(
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

        // Tilemap still takes a Spritesheet& (asset), so deref shared_ptr
        tilemap = std::make_unique<Tilemap>(*tilemap_spritesheet, 16, map_data[0].size(), map_data.size(), map_data);
        tilemap->set_collision_tiles({3});

        // player spritesheet (shared asset)
        player_spritesheet = std::make_shared<Spritesheet>(
            "res/sprites/bw_player.png",
            16,
            16);

        player_spritesheet->define_animation("still", {{0}, 1.0f, true});
        player_spritesheet->define_animation("walk_down", {{1, 2}, 0.2f, true});
        player_spritesheet->define_animation("walk_up", {{7, 8}, 0.2f, true});
        player_spritesheet->define_animation("walk_right", {{4, 5}, 0.2f, true});
        player_spritesheet->define_animation("walk_left", {{9, 10}, 0.2f, true});

        // Player now expects shared_ptr<Spritesheet>
        player = std::make_unique<Player>(player_spritesheet, 16, 16, 16, 16);
        player->play_animation("still");
        player->set_bound_box_dimensions(8, 4);
        player->set_bound_box_offset(4, 12);
        camera.target = player->get_position();

        // tree spritesheet (shared asset)
        tree_spritesheet = std::make_shared<Spritesheet>(
            "res/sprites/tree.png",
            16,
            32);

        tree_spritesheet->define_animation("still", {{0}, 1.0f, true});
        tree_spritesheet->define_animation("inv", {{1}, 0.5f, true});

        for (int i = 0; i < 5; ++i)
        {
            // spawn<Tree> now should pass shared_ptr<Spritesheet> into Tree/Entity
            Tree *tree = spawn<Tree>(tree_spritesheet, 16, 32, 32 + i * 32, 32);
            tree->set_bound_box_dimensions(2, 2);
            tree->set_bound_box_offset(7, 30);
            tree->play_animation("still");
            tree->set_dialog({{"The tree is too tall to climb.", "--", {}}});
            trees.push_back(tree); // trees should be std::vector<Tree*> (non-owning handles)
        }
    }

    TownScene::~TownScene() = default;

    void TownScene::update(float dt)
    {
        PlayScene::update(dt);

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
