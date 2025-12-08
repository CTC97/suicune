#include "menu_item.hpp"

namespace barley
{
    MenuItem::MenuItem(std::variant<std::string, Texture2D> label, int x, int y, std::function<void()> callback)
        : label(std::move(label)), x(x), y(y), callback(std::move(callback)) {}

    bool MenuItem::is_selected() const
    {
        return selected;
    }

    void MenuItem::set_selected(bool selected)
    {
        this->selected = selected;
    }

    bool MenuItem::is_text() const
    {
        return std::holds_alternative<std::string>(label);
    }

    bool MenuItem::is_image() const
    {
        return std::holds_alternative<Texture2D>(label);
    }

    const std::variant<std::string, Texture2D> &MenuItem::get_label() const
    {
        return label;
    }

    int MenuItem::get_x() const
    {
        return x;
    }

    int MenuItem::get_y() const
    {
        return y;
    }

    const std::function<void()> &MenuItem::get_callback() const
    {
        return callback;
    }
}