#pragma once

namespace barley
{

    class Game;

    class Scene
    {
    public:
        explicit Scene(Game &game);
        virtual ~Scene();

        virtual void update(float dt) = 0;
        virtual void draw() = 0;

    protected:
        Game &game;
    };

}
