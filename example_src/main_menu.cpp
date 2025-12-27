#include "main_menu.hpp"
#include "town_scene.hpp"

namespace suicune
{

    MainMenu::MainMenu(Game &game)
        : MenuScene(game)
    {
        play_texture = LoadTexture("res/sprites/play_sprite.png");
        auto play_button = define_transition_option<TownScene>(play_texture, (game.get_window_width() - play_texture.width) / 2, 200);
        play_button->set_clicked_callback(
            []() {

            });
        define_quit_option("Quit", (game.get_window_width() - MeasureText("Quit", 20)) / 2, 300);
    }

    MainMenu::~MainMenu()
    {
        UnloadTexture(play_texture);
    }

    void MainMenu::draw()
    {
        MenuScene::draw();
    }

    void MainMenu::update(float dt)
    {
        MenuScene::update(dt);

        if (IsKeyPressed(KEY_P))
        {
            shake(0.5f, 5.0f);
        }
    }
}
