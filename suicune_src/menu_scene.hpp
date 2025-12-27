#pragma once

#include "scene.hpp"
#include <string>
#include <vector>
#include <functional>
#include "raylib.h"
#include "../suicune_src/game.hpp"
#include <variant>
#include "menu_item.hpp"

namespace suicune
{

    class MenuScene : public Scene
    {
    public:
        MenuScene(Game &game);
        ~MenuScene() override;

        void update(float dt) override;
        void draw() override;

        void cleanup_eoframe() override;

    protected:
        MenuItem *define_option(std::variant<std::string, Texture2D> label, int x, int y, const std::function<void()> &callback);
        template <typename SceneType>
        MenuItem *define_transition_option(std::variant<std::string, Texture2D> label, int x, int y)
        {
            return define_option(label, x, y, [this]()
                                 { this->game.request_scene(std::make_unique<SceneType>(this->game)); });
        }

        MenuItem *define_quit_option(std::variant<std::string, Texture2D> label, int x, int y);

        const std::vector<MenuItem> &get_items() const;
        std::vector<MenuItem> &get_items();

        int get_selected_index() const;
        void set_selected_index(int index);

    private:
        std::vector<MenuItem> items;
        int selected_index = 0;
    };

}
