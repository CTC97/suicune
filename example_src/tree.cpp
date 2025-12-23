#include "tree.hpp"

namespace suicune
{
    Tree::Tree(std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y)
        : Entity(std::move(spritesheet), width, height, x, y)
    {
        set_bound_box_dimensions(2, 2);
        set_bound_box_offset(7, 30);

        set_dialog({{"The tree is too tall to climb.", "--", {}}});

        play_animation("still");
    }

    void Tree::interact()
    {
        inverted = !inverted;

        if (inverted)
        {
            play_animation("inv");
            set_dialog({{"You've changed me.", "--", {}}});
            destroy();
        }
        else
        {
            play_animation("still");
            set_dialog({{"Ah much better.", "--", {}}});
        }

        Entity::interact();
    }
}
