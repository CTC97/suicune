#include "house.hpp"

namespace suicune
{
    House::House(std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y)
        : Entity(std::move(spritesheet), width, height, x, y)
    {
        set_bound_box_dimensions(28, 16);
        set_bound_box_offset(2, 16);

        // TODO : need to pass Scene down to Entity
        // set_collision_callback([this]()
        //                        {
        //                                 TraceLog(LOG_INFO, "Transitioning to Main Menu Scene...");
        //                                 transitioning_scene = true;
        //                                  this->game.request_scene(std::make_unique<MainMenu>(this->game)); });
        // or maybe this should have to be handled at the scene level?
        // provide users full access always everywhere? entity -> scene -> game?
        //          ->>>> yes this allows for better abstraction and cleaner code at higher levels
        //          goal of the engine is to give power at low levels and keep high levels simple
        //                  ->>>>> easy to start up, easy to build complexity

        play_animation("still");
    }
}
