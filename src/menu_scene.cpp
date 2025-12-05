#include "menu_scene.hpp"

namespace barley
{

    MenuScene::MenuScene(Game &game)
        : Scene(game)
    {
    }

    MenuScene::~MenuScene() = default;

    void MenuScene::add_option(const std::string &text, int x, int y, const std::function<void()> &callback)
    {
        options.push_back(MenuItem{text, x, y, callback});
    }

    const std::vector<MenuItem> &MenuScene::get_options() const
    {
        return options;
    }

    std::vector<MenuItem> &MenuScene::get_options()
    {
        return options;
    }

    int MenuScene::get_selected_index() const
    {
        return selected_index;
    }

    void MenuScene::set_selected_index(int index)
    {
        if (options.empty())
        {
            selected_index = 0;
            return;
        }

        if (index < 0)
            index = 0;
        else if (index >= static_cast<int>(options.size()))
            index = static_cast<int>(options.size()) - 1;

        selected_index = index;
    }

    void MenuScene::update(float dt)
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

    void MenuScene::draw()
    {
        DrawText("barley.", 6, game.get_window_height() - 15, 10, WHITE);

        const auto &options = get_options();
        int selected = get_selected_index();

        for (int i = 0; i < static_cast<int>(options.size()); ++i)
        {
            int y = 180 + i * 30;
            Color color = (i == selected) ? MAROON : DARKGRAY;
            DrawText(options[i].text.c_str(), 300, y, 20, color);
        }
    }
}
