#include "tree.hpp"

namespace suicune
{
    Tree::Tree(std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y)
        : Entity(std::move(spritesheet), width, height, x, y)
    {
    }

    void Tree::interact()
    {
        inverted = !inverted;

        if (inverted)
        {
            play_animation("inv");
            set_dialog({{"You've changed me.", "--", {}}});
        }
        else
        {
            play_animation("still");
            set_dialog({{"Ah much better.", "--", {}}});
        }

        Entity::interact();
    }
}
