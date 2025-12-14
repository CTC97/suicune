#include "dream_scene.hpp"
#include "../barley_src/game.hpp"
#include <vector>
#include <random>

namespace barley
{

    bool is_position_valid(int x, int y, const std::vector<std::vector<bool>> &collision_map, int tile_size)
    {
        int tile_x = x / tile_size;
        int tile_y = y / tile_size;

        if (tile_x < 0 || tile_x >= collision_map[0].size() || tile_y < 0 || tile_y >= collision_map.size())
            return false;

        return !collision_map[tile_y][tile_x];
    }

    std::vector<std::vector<int>> generate_grid(int width, int height)
    {
        std::vector<std::vector<int>> grid(height, std::vector<int>(width));

        std::random_device rd;
        std::mt19937 gen(rd());

        // Weighted distribution:
        // 0 = common, 5 = common, 3 = rare
        // You can tune these numbers freely
        std::discrete_distribution<> dist({
            45, // weight for 0
            10, // weight for 3 (sparse)
            45  // weight for 5
        });

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int roll = dist(gen);
                switch (roll)
                {
                case 0:
                    grid[y][x] = 0;
                    break;
                case 1:
                    grid[y][x] = 3;
                    break;
                case 2:
                    grid[y][x] = 5;
                    break;
                }
            }
        }

        return grid;
    }

    DreamScene::DreamScene(Game &game)
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

        std::vector<std::vector<int>> map_data = generate_grid(44, 44);

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

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_x(0, 43);
        std::uniform_int_distribution<> dist_y(0, 43);

        for (int i = 0; i < 10; ++i)
        {
            int x, y;
            do
            {
                x = dist_x(gen) * 16; // Convert to tilemap coordinates
                y = dist_y(gen) * 16;
            } while (!is_position_valid(x, y, entity_collision_map, 16));

            auto nurse_joy = std::make_unique<Entity>(*nurse_joy_spritesheet, x, y);
            nurse_joy->set_dialog({{dialogue_lines[i]}});
            nurse_joy->play_animation("still");

            add_entity(std::move(nurse_joy));
        }
    }

    DreamScene::~DreamScene() = default;

    void DreamScene::update(float dt)
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

    void DreamScene::draw()
    {
        camera.target = player->get_position();
        camera.offset = (Vector2){game.get_window_width() / 2.0f, game.get_window_height() / 2.0f};
        PlayScene::draw();
    }

    Camera2D &DreamScene::get_camera()
    {
        return camera;
    }

}
