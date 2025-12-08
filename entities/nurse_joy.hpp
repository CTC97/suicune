#pragma once

#include "../src/entity.hpp"

namespace barley
{

    class NurseJoy : public Entity
    {
    public:
        NurseJoy(Spritesheet &spritesheet, /*int width, int height,*/ int x, int y);
        ~NurseJoy();

        void interact() override;
    };

}