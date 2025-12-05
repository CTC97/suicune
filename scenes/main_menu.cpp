#include <memory>
#include "main_menu.hpp"
#include "raylib.h"
#include "../src/game.hpp"

namespace barley
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        add_option("Play", 20, 20, []()
                   { printf("Play option selected!\n"); });

        // add_option("Options", 20, 20, [this]()
        //            { this->game.set_scene(std::make_unique<OptionMenu>(this->game)); });

        add_option("Quit", 40, 40, [this]()
                   { this->game.quit(); });
    }

    MainMenu::~MainMenu() = default;

    void MainMenu::draw()
    {
        MenuScene::draw();

        DrawText("Welcome to the Main Menu!", 10, 10, 20, RAYWHITE);
    }
}
