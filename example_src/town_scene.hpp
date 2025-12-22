#pragma once

#include "../suicune_src/play_scene.hpp"
#include "tree.hpp"

namespace suicune
{

    class TownScene : public PlayScene
    {
    public:
        TownScene(Game &game);
        ~TownScene() override;

        void draw() override;
        void update(float dt) override;

    private:
        std::shared_ptr<Spritesheet> tilemap_spritesheet;
        std::shared_ptr<Spritesheet> player_spritesheet;
        std::shared_ptr<Spritesheet> tree_spritesheet;
        std::shared_ptr<Spritesheet> house_spritesheet;

        std::vector<Tree *> trees;
    };

}