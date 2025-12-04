#include <memory>
#include "option_menu.hpp"
#include "raylib.h"
#include "../src/game.hpp"
#include "main_menu.hpp"

namespace barley
{

    OptionMenu::OptionMenu(Game &game)
        : MenuScene(game)
    {
        // For now, just two options

        add_option("Back", [this]()
                   { this->game.set_scene(std::make_unique<MainMenu>(this->game)); });

        add_option("Quit", [this]()
                   { this->game.quit(); });
    }

    OptionMenu::~OptionMenu() = default;

    void OptionMenu::update(float dt)
    {
        (void)dt;

        auto &options = get_options();
        if (options.empty())
            return;

        int index = get_selected_index();
        int count = static_cast<int>(options.size());

        // Move selection down
        if (IsKeyPressed(KEY_DOWN))
        {
            index++;
            if (index >= count)
                index = 0;
            set_selected_index(index);
        }

        // Move selection up
        if (IsKeyPressed(KEY_UP))
        {
            index--;
            if (index < 0)
                index = count - 1;
            set_selected_index(index);
        }

        // Activate selected option
        if (IsKeyPressed(KEY_ENTER))
        {
            const auto &option = options[get_selected_index()];
            if (option.callback)
            {
                option.callback();
            }
        }
    }

    void OptionMenu::draw()
    {
        const auto &options = get_options();
        int selected = get_selected_index();

        DrawText("BARLEY", 280, 80, 40, BLACK);

        for (int i = 0; i < static_cast<int>(options.size()); ++i)
        {
            int y = 180 + i * 30;
            Color color = (i == selected) ? MAROON : DARKGRAY;
            DrawText(options[i].text.c_str(), 300, y, 20, color);
        }
    }

}
