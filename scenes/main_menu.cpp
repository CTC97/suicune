#include <memory>
#include "main_menu.hpp"
#include "raylib.h"
#include "../src/game.hpp"

namespace barley
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        Texture2D playTexture = LoadTexture("res/sprites/play_sprite.png");
        add_option(playTexture, (game.get_window_width() - playTexture.width) / 2, 200, []()
                   { printf("Play option selected!\n"); });

        // add_option("Options", 20, 20, [this]()
        //            { this->game.set_scene(std::make_unique<OptionMenu>(this->game)); });

        add_option("Quit", (game.get_window_width() - MeasureText("Quit", 20)) / 2, 300, [this]()
                   { this->game.quit(); });
    }

    MainMenu::~MainMenu() = default;

    void MainMenu::draw()
    {
        MenuScene::draw();
    }
}
