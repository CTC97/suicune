#pragma once

#include <memory>
#include "scene.hpp"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"

namespace barley
{

    class PlayScene : public Scene
    {
    public:
        explicit PlayScene(Game &game);
        ~PlayScene() override;

        void update(float dt) override;
        void draw() override;

        void set_tilemap(std::unique_ptr<Tilemap> map);

    protected:
        // Derived scenes must provide a camera
        virtual Camera2D &get_camera() = 0;

        std::unique_ptr<Tilemap> tilemap;

        std::unique_ptr<Player> player;
    };

}