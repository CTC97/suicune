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

        initialize_entity_collision_map();

        player_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/bw_player.png",
            16,
            16);

        player_spritesheet->define_animation("still", {{0}, 1.0f, true});
        player_spritesheet->define_animation("walk_down", {{1, 2}, 0.2f, true});
        player_spritesheet->define_animation("walk_up", {{7, 8}, 0.2f, true});
        player_spritesheet->define_animation("walk_right", {{4, 5}, 0.2f, true});
        player_spritesheet->define_animation("walk_left", {{9, 10}, 0.2f, true});

        player = std::make_unique<Player>(*player_spritesheet, 16, 16);
        player->play_animation("still");
        camera.target = player->get_position();

        nurse_joy_spritesheet = std::make_unique<Spritesheet>(
            "res/sprites/Sprite-0001.png",
            16,
            16);
        nurse_joy_spritesheet->define_animation("still", {{0}, 1.0f, true});

        auto nurse_joy = std::make_unique<Entity>(*nurse_joy_spritesheet, 64, 64);
        nurse_joy->set_dialog({{
            "Hi-hi! Oh—sorry, I was just humming again, wasn't I? The market always makes me nervous and excited at the same time, like all the colors are whispering secrets and daring me to keep mine tucked safely away. Not that it's a bad secret, mind you—just a personal one, the kind you fold up carefully and keep close to your heart while you smile and help everyone find what they're looking for. I like being seen as just me: the girl who laughs too loud, trips over nothing, and always remembers your favorite candy. Sometimes I wonder if people can tell there's more to me than meets the eye, but then someone waves and asks about the weather, and I relax again. Maybe one day I'll be brave enough to share everything, or maybe I won't need to, because I'm happy like this—walking through town, cheeks warm, heart steady, knowing that even if the world doesn't know all of me, it still lets me belong.",
        }});
        nurse_joy->play_animation("still");

        add_entity(std::move(nurse_joy));
    }

    TownScene::~TownScene() = default;

    void TownScene::update(float dt)
    {
        if (IsKeyDown(KEY_RIGHT))
            player->play_animation("walk_right");
        else if (IsKeyDown(KEY_LEFT))
            player->play_animation("walk_left");
        else if (IsKeyDown(KEY_UP))
            player->play_animation("walk_up");
        else if (IsKeyDown(KEY_DOWN))
            player->play_animation("walk_down");
        else
            player->play_animation("still");
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
