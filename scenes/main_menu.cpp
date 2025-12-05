#include "main_menu.hpp"
#include "town_scene.hpp"

namespace barley
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        bg_texture = LoadTexture("res/sprites/bg_800_450.png");

        Texture2D playTexture = LoadTexture("res/sprites/play_sprite.png");
        add_option(playTexture, (game.get_window_width() - playTexture.width) / 2, 200, [this]()
                   { this->game.set_scene(std::make_unique<TownScene>(this->game)); });

        add_option("Quit", (game.get_window_width() - MeasureText("Quit", 20)) / 2, 300, [this]()
                   { this->game.quit(); });
    }

    MainMenu::~MainMenu() = default;

    void MainMenu::draw()
    {
        DrawTexture(bg_texture, 0, 0, WHITE);
        MenuScene::draw();
    }
}
