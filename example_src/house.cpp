#include "house.hpp"

namespace suicune
{
    House::House(Scene *scene, std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y)
        : Entity(scene, std::move(spritesheet), width, height, x, y)
    {
        type = "house";

        set_bound_box_dimensions(28, 16);
        set_bound_box_offset(2, 16);

        // set_collision_callback([scene]()
        //                        { scene->request_scene(std::make_unique<MainMenu>(scene->get_game())); });
        // provide users full access always everywhere? entity -> scene -> game?
        //          ->>>> yes this allows for better abstraction and cleaner code at higher levels
        //          goal of the engine is to give power at low levels and keep high levels simple
        //                  ->>>>> easy to start up, easy to build complexity

        play_animation("still");
    }
}
