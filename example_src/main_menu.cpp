#include "main_menu.hpp"
#include "town_scene.hpp"

namespace suicune
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        play_texture = LoadTexture("res/sprites/play_sprite.png");
        add_transition_option<TownScene>(play_texture, (game.get_window_width() - play_texture.width) / 2, 200);

        add_quit_option("Quit", (game.get_window_width() - MeasureText("Quit", 20)) / 2, 300);
    }

    MainMenu::~MainMenu()
    {
        UnloadTexture(play_texture);
    }

    void MainMenu::draw()
    {
        MenuScene::draw();
    }
}
