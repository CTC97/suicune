#pragma once

#include "scene.hpp"
#include <string>
#include <vector>
#include <functional>
#include "raylib.h"
#include "../src/game.hpp"

namespace barley
{

    struct MenuItem
    {
        std::string text;
        int x, y;
        std::function<void()> callback;
    };

    class MenuScene : public Scene
    {
    public:
        MenuScene(Game &game);
        ~MenuScene() override;

        void update(float dt) override;
        void draw() override;

    protected:
        void add_option(const std::string &text, int x, int y, const std::function<void()> &callback);

        const std::vector<MenuItem> &get_options() const;
        std::vector<MenuItem> &get_options();

        int get_selected_index() const;
        void set_selected_index(int index);

    private:
        std::vector<MenuItem> options;
        int selected_index = 0;
    };

}
