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
        if (player)
        {
            player->update(*tilemap);
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

}