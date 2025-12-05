#include "menu_scene.hpp"

namespace barley
{

    MenuScene::MenuScene(Game &game)
        : Scene(game)
    {
    }

    MenuScene::~MenuScene()
    {
        for (auto &item : items)
        {
            if (item.is_image())
            {
                UnloadTexture(std::get<Texture2D>(item.get_label()));
            }
        }
    }

    void MenuScene::add_option(std::variant<std::string, Texture2D> label, int x, int y, const std::function<void()> &callback)
    {
        items.emplace_back(label, x, y, callback);
    }

    const std::vector<MenuItem> &MenuScene::get_items() const
    {
        return items;
    }

    std::vector<MenuItem> &MenuScene::get_items()
    {
        return items;
    }

    int MenuScene::get_selected_index() const
    {
        return selected_index;
    }

    void MenuScene::set_selected_index(int index)
    {
        if (items.empty())
        {
            selected_index = 0;
            return;
        }

        if (index < 0)
            index = 0;
        else if (index >= static_cast<int>(items.size()))
            index = static_cast<int>(items.size()) - 1;

        selected_index = index;
    }

    void MenuScene::update(float dt)
    {
        (void)dt;

        auto &items = get_items();
        if (items.empty())
            return;

        int index = get_selected_index();
        int count = static_cast<int>(items.size());

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
        for (size_t i = 0; i < items.size(); ++i)
        {
            if (static_cast<int>(i) != get_selected_index())
            {
                items[i].set_selected(false);
            }
            else
            {
                items[i].set_selected(true);
            }
        }

        // Activate selected option
        if (IsKeyPressed(KEY_ENTER))
        {
            const auto &option = items[get_selected_index()];
            if (option.get_callback())
            {
                option.get_callback()();
            }
        }
    }

    void MenuScene::draw()
    {
        const auto &items = get_items();

        for (const auto &item : items)
        {
            if (item.is_text())
            {
                DrawText(std::get<std::string>(item.get_label()).c_str(), item.get_x(), item.get_y(), 20, WHITE);
            }
            else if (item.is_image())
            {
                DrawTexture(std::get<Texture2D>(item.get_label()), item.get_x(), item.get_y(), WHITE);
            }

            if (item.is_selected())
            {
                DrawText(">", item.get_x() - 30, item.get_y(), 20, WHITE);
            }
        }

        Scene::draw();
    }
}
