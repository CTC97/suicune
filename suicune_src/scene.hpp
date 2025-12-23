#pragma once

#include "raylib.h"
#include "spritesheet.hpp"

namespace suicune
{

    class Game;

    class Scene
    {
    public:
        explicit Scene(Game &game);
        virtual ~Scene();

        virtual void update(float dt);
        virtual void draw();

        virtual void cleanup_eoframe() = 0;

    protected:
        Game &game;
        bool transitioning_scene = false;

        virtual std::shared_ptr<Spritesheet> define_spritesheet(const char *file_path, int frame_width, int frame_height);

    private:
        std::unordered_map<std::string, std::shared_ptr<Spritesheet>> spritesheets;
        void unload_spritesheets();
    };

}
