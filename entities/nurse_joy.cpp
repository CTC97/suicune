#include "nurse_joy.hpp"
#include <iostream>

namespace barley
{

    NurseJoy::NurseJoy(Spritesheet &spritesheet, int x, int y)
        : Entity(spritesheet, x, y)
    {
    }

    NurseJoy::~NurseJoy()
    {
    }

    void NurseJoy::interact()
    {
        // Interaction logic
        std::cout << "Nurse Joy: Welcome! Let me heal your Pokémon." << std::endl;
    }

}