#include "main_menu.hpp"
#include "dream_scene.hpp"

namespace suicune
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        Texture2D playTexture = LoadTexture("res/sprites/play_sprite.png");
        add_transition_option<DreamScene>(playTexture, (game.get_window_width() - playTexture.width) / 2, 200);

        add_quit_option("Quit", (game.get_window_width() - MeasureText("Quit", 20)) / 2, 300);
    }

    MainMenu::~MainMenu() = default;

    void MainMenu::draw()
    {
        MenuScene::draw();
    }
}
