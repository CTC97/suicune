#include "menu_item.hpp"

namespace suicune
{

    std::pair<int, int> calculateDimensions(const std::variant<std::string, Texture2D> &input, int fontSize)
    {
        if (std::holds_alternative<std::string>(input))
        {
            const std::string &text = std::get<std::string>(input);
            int width = MeasureText(text.c_str(), fontSize); // Calculate text width
            int height = fontSize;                           // Font size as height
            return {width, height};
        }
        else if (std::holds_alternative<Texture2D>(input))
        {
            const Texture2D &texture = std::get<Texture2D>(input);
            return {texture.width, texture.height}; // Use texture dimensions
        }
        return {0, 0}; // Default fallback
    }

    MenuItem::MenuItem(Scene *scene, std::variant<std::string, Texture2D> label, int x, int y, std::function<void()> callback)
        : label(std::move(label)), x(x), y(y), callback(std::move(callback)), interactable(scene, 0, 0, x, y)
    {
        std::tie(width, height) = calculateDimensions(this->label, scene->get_game().get_dialog_manager().get_font_size());

        interactable.set_clickable(true);
    }

    MenuItem::~MenuItem() {};

    void MenuItem::update(float dt)
    {
        interactable.update(dt, width, height, x, y);
    }

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

    void MenuItem::set_clicked_callback(std::function<void()> callback)
    {
        interactable.set_clicked_callback(std::move(callback));
    }
}