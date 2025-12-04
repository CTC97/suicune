#include "menu_scene.hpp"

namespace barley
{

    MenuScene::MenuScene(Game &game)
        : Scene(game)
    {
    }

    MenuScene::~MenuScene() = default;

    void MenuScene::add_option(const std::string &text, const std::function<void()> &callback)
    {
        options.push_back(MenuOption{text, callback});
    }

    const std::vector<MenuOption> &MenuScene::get_options() const
    {
        return options;
    }

    std::vector<MenuOption> &MenuScene::get_options()
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

}
