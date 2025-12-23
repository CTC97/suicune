#pragma once

#include <memory>
#include <vector>
#include "scene.hpp"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"
#include "entity.hpp"
#include "util.hpp"

namespace suicune
{

    class PlayScene : public Scene
    {
    public:
        explicit PlayScene(Game &game);
        ~PlayScene() override;

        void update(float dt) override;
        void draw() override;

        Entity *add_entity(std::unique_ptr<Entity> entity);
        template <typename T, typename... Args>
        T *define_entity(Args &&...args)
        {
            static_assert(std::is_base_of_v<Entity, T>, "spawn<T> requires T to derive from Entity");
            auto entity = std::make_unique<T>(std::forward<Args>(args)...);
            T *ptr = entity.get();
            add_entity(std::move(entity));

            return ptr;
        }

        void remove_entity(Entity *entity);

        void check_interaction();

        void cleanup_eoframe() override;

    protected:
        // Derived scenes must provide a camera
        virtual Camera2D &get_camera() = 0;

        std::unique_ptr<Tilemap> tilemap;
        std::unique_ptr<Player> player;

        std::vector<std::unique_ptr<Entity>> entities;
    };

}