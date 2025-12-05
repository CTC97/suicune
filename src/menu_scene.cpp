#include "menu_scene.hpp"

namespace barley
{

    MenuScene::MenuScene(Game &game)
        : Scene(game)
    {
    }

    MenuScene::~MenuScene() = default;

    void MenuScene::add_option(std::variant<std::string, Texture2D> label, int x, int y, const std::function<void()> &callback)
    {
        options.emplace_back(label, x, y, callback);
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

        // Update selection states
        for (size_t i = 0; i < options.size(); ++i)
        {
            if (static_cast<int>(i) != get_selected_index())
            {
                options[i].set_selected(false);
            }
            else
            {
                options[i].set_selected(true);
            }
        }

        // Activate selected option
        if (IsKeyPressed(KEY_ENTER))
        {
            const auto &option = options[get_selected_index()];
            if (option.get_callback())
            {
                option.get_callback()();
            }
        }
    }

    void MenuScene::draw()
    {
        DrawText("barley.", 6, game.get_window_height() - 15, 10, WHITE);

        const auto &options = get_options();

        for (const auto &item : options)
        {
            if (item.is_text())
            {
                DrawText(std::get<std::string>(item.get_label()).c_str(), item.get_x(), item.get_y(), 20, WHITE);
            }
            else if (item.is_image())
            {
                printf("Drawing texture at (%d, %d)\n", item.get_x(), item.get_y());
                DrawTexture(std::get<Texture2D>(item.get_label()), item.get_x(), item.get_y(), WHITE);
            }

            if (item.is_selected())
            {
                DrawText(">", item.get_x() - 30, item.get_y(), 20, WHITE);
            }
        }
    }
}
