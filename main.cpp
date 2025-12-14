#include "barley_src/game.hpp"
#include "scenes/main_menu.hpp"

using namespace barley;

int main()
{
    Game game("Barley Test", 1200, 700, 16);

    game.set_scene(std::make_unique<MainMenu>(game));

    game.get_dialog_manager().set_dialog_box_texture("res/sprites/dialog_box.png");
    game.get_dialog_manager().set_text_padding(40, 34);
    game.get_dialog_manager().set_font(36, 4.0f);

    game.run();
}
