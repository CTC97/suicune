// dialog_manager.cpp
#include "dialog_manager.hpp"
#include <algorithm>

namespace suicune
{
    DialogManager::DialogManager()
        : current_node_index(-1), active(false), text_speed(0.05f), text_timer(0.0f), current_page_index(0), reveal_char_index(0) {}

    DialogManager::~DialogManager()
    {
        if (dialog_font.texture.id != GetFontDefault().texture.id)
        {
            UnloadFont(dialog_font);
        }

        if (dialog_box_texture.id != 0)
        {
            UnloadTexture(dialog_box_texture);
        }
    }

    void DialogManager::set_font(const std::string &font_path, int size, float spacing)
    {
        // Unload the previous font if it was custom
        Font font = LoadFontEx(font_path.c_str(), size, nullptr, 0);

        if (dialog_font.texture.id != GetFontDefault().texture.id)
        {
            UnloadFont(dialog_font);
        }

        dialog_font = font;
        font_size = size;
        font_spacing = spacing;
    }

    int DialogManager::get_font_size()
    {
        return font_size;
    }

    float DialogManager::line_height() const
    {
        return MeasureTextEx(dialog_font, "A", static_cast<float>(font_size), font_spacing).y;
    }

    float DialogManager::usable_text_width() const
    {
        return std::max(0.0f, get_dialog_box_texture().width - text_padding_x * 2);
    }

    int DialogManager::compute_max_lines_per_page() const
    {
        float usable_h = std::max(0.0f, get_dialog_box_texture().height - text_padding_y);
        float lh = line_height();
        if (lh <= 0.0f)
            return 1;
        int max_lines = (int)(usable_h / lh);
        return std::max(1, max_lines);
    }

    void DialogManager::start_dialog(const std::vector<DialogNode> &nodes)
    {
        dialog_nodes = nodes;
        current_node_index = 0;
        active = true;

        build_pages_for_current_node();

        // Reset reveal
        visible_text.clear();
        reveal_char_index = 0;
        text_timer = 0.0f;
    }

    void DialogManager::build_pages_for_current_node()
    {
        if (current_node_index < 0 || current_node_index >= (int)dialog_nodes.size())
        {
            pages.clear();
            current_page_index = 0;
            return;
        }

        full_text = dialog_nodes[current_node_index].text;

        const float max_w = usable_text_width();
        auto lines = wrap_text_to_lines(full_text, max_w);

        const int max_lines_per_page = compute_max_lines_per_page();
        pages = split_lines_into_pages(lines, max_lines_per_page);

        current_page_index = 0;
    }

    // Word-wrap text into lines. Preserves explicit '\n' as forced line breaks.
    std::vector<std::string> DialogManager::wrap_text_to_lines(const std::string &text, float max_width) const
    {
        std::vector<std::string> lines;
        std::string current_line;
        std::istringstream words(text);
        std::string word;

        while (words >> word)
        {
            std::string test_line = current_line.empty() ? word : current_line + " " + word;

            // Use MeasureTextEx to calculate the width of the text
            float text_width = MeasureTextEx(dialog_font, test_line.c_str(), static_cast<float>(font_size), font_spacing).x;

            if (text_width > max_width)
            {
                // Add the current line to the list and start a new line
                lines.push_back(current_line);
                current_line = word;
            }
            else
            {
                current_line = test_line;
            }
        }

        // Add the last line
        if (!current_line.empty())
        {
            lines.push_back(current_line);
        }

        return lines;
    }
    // Groups N lines per page and joins them with '\n'
    std::vector<std::string> DialogManager::split_lines_into_pages(const std::vector<std::string> &lines, int max_lines) const
    {
        std::vector<std::string> out;
        if (max_lines <= 0)
            max_lines = 1;

        for (size_t i = 0; i < lines.size();)
        {
            std::string page;
            int count = 0;

            while (i < lines.size() && count < max_lines)
            {
                if (!page.empty())
                    page += "\n";
                page += lines[i];
                i++;
                count++;
            }

            out.push_back(page);
        }

        if (out.empty())
            out.push_back("");
        return out;
    }

    void DialogManager::update(float dt)
    {
        if (!active)
            return;

        if (current_node_index < 0 || current_node_index >= (int)dialog_nodes.size())
        {
            active = false;
            return;
        }

        if (pages.empty())
        {
            build_pages_for_current_node();
        }

        const std::string &page_text = pages[current_page_index];

        // Typewriter reveal on current page
        if (reveal_char_index < (int)page_text.size())
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                visible_text = page_text;
                reveal_char_index = (int)page_text.size();
            }
            else
            {
                text_timer += dt;
                if (text_timer >= text_speed)
                {
                    text_timer = 0.0f;
                    visible_text.push_back(page_text[reveal_char_index]);
                    reveal_char_index++;
                }
            }
            return;
        }

        // Page is fully revealed: Enter advances page or node
        if (IsKeyPressed(KEY_ENTER))
        {
            // If more pages in this same node, go next page.
            if (current_page_index + 1 < (int)pages.size())
            {
                current_page_index++;
                visible_text.clear();
                reveal_char_index = 0;
                text_timer = 0.0f;
                return;
            }

            // Otherwise, advance to next node
            step_dialog();
        }
    }

    void DialogManager::draw(float base_x, float base_y)
    {
        if (!active)
            return;

        if (dialog_box_texture.id != 0)
        {
            DrawTexture(dialog_box_texture, (int)base_x, (int)base_y, WHITE);
        }
        else
        {
            throw std::runtime_error("Dialog box texture missing.");
        }

        DrawTextEx(
            dialog_font,                                        // Use the custom font
            visible_text.c_str(),                               // The text to draw
            {base_x + text_padding_x, base_y + text_padding_y}, // Position
            static_cast<float>(font_size),                      // Font size
            font_spacing,                                       // Spacing between characters
            WHITE                                               // Text color
        );

        if (current_page_index < static_cast<int>(pages.size()) - 1)
        {
            // add animations for more pages indicator
        }

        // Options: only show when node is on its final page and fully revealed
        // const DialogNode &node = dialog_nodes[current_node_index];
        // const bool on_last_page = (current_page_index == (int)pages.size() - 1);

        // if (!node.options.empty() && on_last_page)
        // {
        //     const std::string &page_text = pages[current_page_index];
        //     const bool page_fully_revealed = (reveal_char_index >= (int)page_text.size());

        //     if (page_fully_revealed)
        //     {
        //         int y_offset = (int)(text_padding_y + (compute_max_lines_per_page() + 1) * line_height());
        //         for (const auto &option : node.options)
        //         {
        //             DrawText(option.label.c_str(),
        //                      (int)(base_x + text_padding_x),
        //                      (int)(base_y + y_offset),
        //                      font_size,
        //                      GRAY);
        //             y_offset += (int)line_height();
        //         }
        //     }
        // }
    }

    bool DialogManager::is_active() const
    {
        return active;
    }

    void DialogManager::step_dialog()
    {
        if (current_node_index < 0 || current_node_index >= (int)dialog_nodes.size())
        {
            active = false;
            return;
        }

        const DialogNode &current_node = dialog_nodes[current_node_index];

        if (!current_node.options.empty())
        {
            // placeholder selection
            current_node_index = current_node.options[0].next_node_index;
        }
        else
        {
            current_node_index++;
        }

        if (current_node_index < 0 || current_node_index >= (int)dialog_nodes.size())
        {
            active = false;
            return;
        }

        // Rebuild pages for the new node
        build_pages_for_current_node();

        // Reset reveal state for page 0
        visible_text.clear();
        reveal_char_index = 0;
        text_timer = 0.0f;
    }

    const DialogNode *DialogManager::get_current_node() const
    {
        if (current_node_index >= 0 && current_node_index < (int)dialog_nodes.size())
            return &dialog_nodes[current_node_index];
        return nullptr;
    }

    void DialogManager::set_dialog_box_texture(const std::string &texture_path)
    {
        dialog_box_texture = LoadTexture(texture_path.c_str());
    }

    Texture2D DialogManager::get_dialog_box_texture() const
    {
        return dialog_box_texture;
    }

    void DialogManager::set_text_padding(float padding_x, float padding_y)
    {
        text_padding_x = padding_x;
        text_padding_y = padding_y;
    }
}
