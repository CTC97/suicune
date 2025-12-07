#pragma once

#include <memory>
#include <vector>
#include "scene.hpp"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"
#include "entity.hpp"

namespace barley
{

    class PlayScene : public Scene
    {
    public:
        explicit PlayScene(Game &game);
        ~PlayScene() override;

        void update(float dt) override;
        void draw() override;

        void add_entity(std::unique_ptr<Entity> entity);
        void remove_entity(Entity *entity);

    protected:
        // Derived scenes must provide a camera
        virtual Camera2D &get_camera() = 0;

        std::unique_ptr<Tilemap> tilemap;
        std::unique_ptr<Player> player;

        std::vector<std::unique_ptr<Entity>> entities;
    };

}