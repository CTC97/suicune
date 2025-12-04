#include "src/game.hpp"
#include "scenes/main_menu.hpp"

using namespace barley;

int main()
{
    Game game("Barley Test", 800, 450);

    game.set_scene(std::make_unique<MainMenu>(game));

    game.run();
}
