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

        for (const auto &entity : entities)
        {
            const int tile_size = game.get_tile_size();
            const int entity_tile_x = static_cast<int>(entity->get_position().x) / tile_size;
            const int entity_tile_y = static_cast<int>(entity->get_position().y) / tile_size;

            entity_collision_map[entity_tile_x][entity_tile_y] = true;

            entity->update(dt);
        }

        if (player)
        {
            player->update(dt, *tilemap, entity_collision_map);
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            check_interaction();
        }
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

        if (player)
        {
            player->draw();
        }

        for (const auto &entity : entities)
        {
            entity->draw();
        }

        EndMode2D();
    }

    void PlayScene::check_interaction()
    {

        const int current_player_direction = player->get_current_direction();
        Vector2 player_pos = player->get_position();

        int player_tile_x = static_cast<int>(player_pos.x) / game.get_tile_size();
        int player_tile_y = static_cast<int>(player_pos.y) / game.get_tile_size();

        int target_x = player_tile_x;
        int target_y = player_tile_y;

        switch (current_player_direction)
        {
        case 0:
            target_y -= 1;
            break;
        case 1:
            target_y += 1;
            break;
        case 2:
            target_x -= 1;
            break;
        case 3:
            target_x += 1;
            break;
        }

        auto target_search = std::find_if(
            entities.begin(),
            entities.end(),
            [this, target_x, target_y](const std::unique_ptr<Entity> &e)
            {
                const Vector2 pos = e->get_position();
                int tile_x = static_cast<int>(pos.x) / game.get_tile_size();
                int tile_y = static_cast<int>(pos.y) / game.get_tile_size();

                return std::abs(tile_x - target_x) < 2 && std::abs(tile_y - target_y) < 2;
            });

        if (target_search != entities.end())
        {
            Entity &target_entity = **target_search;
            target_entity.interact();
        }
    }

    void PlayScene::add_entity(std::unique_ptr<Entity> entity)
    {
        entities.push_back(std::move(entity));
    }

    void PlayScene::remove_entity(Entity *entity)
    {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
                                      [entity](const std::unique_ptr<Entity> &e)
                                      { return e.get() == entity; }),
                       entities.end());
    }

    void PlayScene::initialize_entity_collision_map()
    {
        entity_collision_map.resize(tilemap->get_map_height(), std::vector<bool>(tilemap->get_map_width(), false));
    }

}