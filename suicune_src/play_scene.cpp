#include "play_scene.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"

namespace suicune
{

    PlayScene::PlayScene(Game &game)
        : Scene(game)
    {
    }

    PlayScene::~PlayScene()
    {
    }

    void PlayScene::update(float dt)
    {
        Scene::update(dt);

        DialogManager *dialog_manager = &game.get_dialog_manager();

        if (!dialog_manager->is_active())
        {
            for (const auto &entity : entities)
            {
                entity->update(dt);
            }

            if (player)
                player->update(dt, *tilemap, entities);

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
        Scene::setup_draw();

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

        Scene::cleanup_draw();

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
    }

    void PlayScene::cleanup_eoframe()
    {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
                                      [](const std::unique_ptr<Entity> &e)
                                      { return !e->is_alive(); }),
                       entities.end());
    }

    void PlayScene::check_interaction()
    {
        if (!player)
            return;

        const BoundBox pb = player->get_bound_box();

        // Facing unit vector (pixel space)
        Vector2 facing = {0.0f, 0.0f};
        switch (player->get_current_direction())
        {
        case UP:
            facing = {0.0f, -1.0f};
            break;
        case DOWN:
            facing = {0.0f, 1.0f};
            break;
        case LEFT:
            facing = {-1.0f, 0.0f};
            break;
        case RIGHT:
            facing = {1.0f, 0.0f};
            break;
        }

        // Start at the center of the player's bound box
        Vector2 p = {
            pb.x + pb.width / 2.0f,
            pb.y + pb.height / 2.0f};

        // Move to the *front edge* of the player's bound box
        float half_extent =
            (facing.x != 0.0f) ? (pb.width / 2.0f) : (pb.height / 2.0f);

        p.x += facing.x * half_extent;
        p.y += facing.y * half_extent;

        // Small reach beyond the edge (tune this)
        const float reach_px = 4.0f;
        p.x += facing.x * reach_px;
        p.y += facing.y * reach_px;

        // How forgiving to be around the interact point
        const float max_interact_distance = 4.0f; // keep small now

        Entity *best = nullptr;
        float best_dist = max_interact_distance;

        for (const auto &e : entities)
        {
            if (!e)
                continue;

            BoundBox bound_box = e->get_bound_box();
            float d = point_rect_distance(p, bound_box);
            if (d <= best_dist)
            {
                best_dist = d;
                best = e.get();
            }
        }

        if (best)
            best->interact();
    }

    Entity *PlayScene::add_entity(std::unique_ptr<Entity> entity)
    {
        entity->set_dialog_manager(&game.get_dialog_manager());
        Entity *ptr = entity.get();
        entities.push_back(std::move(entity));
        return ptr;
    }

    void PlayScene::remove_entity(Entity *entity)
    {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
                                      [entity](const std::unique_ptr<Entity> &e)
                                      { return e.get() == entity; }),
                       entities.end());
    }

    void PlayScene::center_on_entity(Entity *entity)
    {
        if (!entity)
            return;

        camera.target = Vector2Add(entity->get_position(), Vector2{entity->get_width() / 2.0f, entity->get_height() / 2.0f});
    }

    void PlayScene::center_on_player()
    {
        center_on_entity(player.get());
    }

}