#pragma once

#include <string>
#include <variant>
#include <functional>
#include "raylib.h"
#include "scene.hpp"
#include "interactable.hpp"

namespace suicune
{
    class MenuItem
    {
    public:
        MenuItem(Scene *scene, std::variant<std::string, Texture2D> label, int x, int y, std::function<void()> callback);
        virtual ~MenuItem();

        bool is_selected() const;
        void set_selected(bool selected);

        bool is_text() const;
        bool is_image() const;

        const std::variant<std::string, Texture2D> &get_label() const;
        int get_x() const;
        int get_y() const;
        const std::function<void()> &get_callback() const;

        virtual void update(float dt);

        void set_clicked_callback(std::function<void()> callback);

    private:
        std::variant<std::string, Texture2D> label;
        int x, y;
        std::function<void()> callback;
        int width, height;

        Interactable interactable;

        bool selected = false;
    };
}