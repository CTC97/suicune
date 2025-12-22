#pragma once

#include <memory>
#include "raylib.h"

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

        void set_shader(Shader shader);
        void clear_shader();

    protected:
        Game &game;
        bool transitioning_scene = false;

        std::shared_ptr<Shader> shader;
    };

}
