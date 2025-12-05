#pragma once

#include <string>
#include <variant>
#include <functional>
#include "raylib.h"

namespace barley
{
    class MenuItem
    {
    public:
        MenuItem(std::variant<std::string, Texture2D> label, int x, int y, std::function<void()> callback);

        bool is_selected() const;
        void set_selected(bool selected);

        bool is_text() const;
        bool is_image() const;

        const std::variant<std::string, Texture2D> &get_label() const;
        int get_x() const;
        int get_y() const;
        const std::function<void()> &get_callback() const;

    private:
        std::variant<std::string, Texture2D> label;
        int x, y;
        std::function<void()> callback;
        bool selected = false;
    };
}