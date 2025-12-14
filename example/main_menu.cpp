#include "main_menu.hpp"
#include "dream_scene.hpp"

namespace suicune
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        Texture2D playTexture = LoadTexture("res/sprites/play_sprite.png");
        add_option(playTexture, (game.get_window_width() - playTexture.width) / 2, 200, [this]()
                   { this->game.set_scene(std::make_unique<DreamScene>(this->game)); });

        add_option("Quit", (game.get_window_width() - MeasureText("Quit", 20)) / 2, 300, [this]()
                   { this->game.quit(); });
    }

    MainMenu::~MainMenu() = default;

    void MainMenu::draw()
    {
        MenuScene::draw();
    }
}
