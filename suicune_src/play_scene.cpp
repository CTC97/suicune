#include "play_scene.hpp"
#include "game.hpp"
#include "raylib.h"

namespace suicune
{

    PlayScene::PlayScene(Game &game)
        : Scene(game)
    {
    }

    PlayScene::~PlayScene() = default;

    void PlayScene::update(float dt)
    {
        (void)dt;

        DialogManager *dialog_manager = &game.get_dialog_manager();

        if (!dialog_manager->is_active())
        {

            std::vector<BoundBox> entity_bound_boxes;

            for (const auto &entity : entities)
            {
                entity->update(dt);
                entity_bound_boxes.push_back(entity->get_bound_box());
            }

            if (player)
                player->update(dt, *tilemap, entity_bound_boxes);

            if (IsKeyPressed(KEY_ENTER))
                check_interaction();
        }
        else
        {
            dialog_manager->update(dt);
        }
    }

    void PlayScene::draw()
    {

        Camera2D &camera = get_camera();

        BeginMode2D(camera);

        if (tilemap)
            tilemap->draw();

        std::vector<std::pair<float, Entity *>> entity_draw_list;
        for (const auto &entity : entities)
            entity_draw_list.emplace_back(entity->get_bound_box().y + entity->get_bound_box().height / 2.0f, entity.get());

        if (player)
            entity_draw_list.emplace_back(player->get_bound_box().y + player->get_bound_box().height / 2.0f, player.get());

        std::sort(entity_draw_list.begin(), entity_draw_list.end(),
                  [](const auto &a, const auto &b)
                  { return a.first < b.first; });

        for (const auto &entity : entity_draw_list)
            entity.second->draw();

        EndMode2D();

        DialogManager *dialog_manager = &game.get_dialog_manager();
        if (dialog_manager->is_active())
        {
            const int window_width = GetScreenWidth();
            const int window_height = GetScreenHeight();
            const int dialog_box_width = dialog_manager->get_dialog_box_texture().width;
            const int dialog_box_height = dialog_manager->get_dialog_box_texture().height;

            dialog_manager->draw(
                (window_width - dialog_box_width) / 2.0f,
                window_height - dialog_box_height - 20.0f);
        }

        Scene::draw();
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
        entity->set_dialog_manager(&game.get_dialog_manager());
        entities.push_back(std::move(entity));
    }

    void PlayScene::remove_entity(Entity *entity)
    {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
                                      [entity](const std::unique_ptr<Entity> &e)
                                      { return e.get() == entity; }),
                       entities.end());
    }

}