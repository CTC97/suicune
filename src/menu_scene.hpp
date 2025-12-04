#pragma once

#include "scene.hpp"
#include <string>
#include <vector>
#include <functional>

namespace barley
{

    struct MenuOption
    {
        std::string text;
        std::function<void()> callback;
    };

    class MenuScene : public Scene
    {
    public:
        MenuScene(Game &game);
        ~MenuScene() override;

    protected:
        void add_option(const std::string &text, const std::function<void()> &callback);

        const std::vector<MenuOption> &get_options() const;
        std::vector<MenuOption> &get_options();

        int get_selected_index() const;
        void set_selected_index(int index);

    private:
        std::vector<MenuOption> options;
        int selected_index = 0;
    };

}
