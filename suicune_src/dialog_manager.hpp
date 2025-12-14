#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "raylib.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace suicune
{
    struct DialogOption
    {
        std::string label;
        int next_node_index;
    };

    struct DialogNode
    {
        std::string text;
        std::string speaker;
        std::vector<DialogOption> options;
    };

    class DialogManager
    {
    public:
        DialogManager();
        ~DialogManager();

        void start_dialog(const std::vector<DialogNode> &nodes);
        void update(float dt);
        void draw(float base_x, float base_y);
        bool is_active() const;

        const DialogNode *get_current_node() const;

        void set_dialog_box_texture(const std::string &texture_path);
        Texture2D get_dialog_box_texture() const;

        void set_text_padding(float padding_x, float padding_y);

        void set_font(const std::string &font_path, int font_size, float spacing = 1.0f);

    private:
        std::vector<DialogNode> dialog_nodes;
        int current_node_index;
        bool active;

        // Typewriter
        float text_speed;
        float text_timer;

        // Layout
        Font dialog_font = GetFontDefault(); // Default font
        int font_size = 20;
        float font_spacing = 1.0f;

        float text_padding_x = 0;
        float text_padding_y = 0;

        Texture2D dialog_box_texture;

        // Paging state
        std::string full_text;
        std::vector<std::string> pages;
        int current_page_index = 0;

        std::string visible_text;
        int reveal_char_index = 0;

        void step_dialog();

        void build_pages_for_current_node();
        std::vector<std::string> wrap_text_to_lines(const std::string &text, float max_width) const;
        std::vector<std::string> split_lines_into_pages(const std::vector<std::string> &lines, int max_lines) const;
        int compute_max_lines_per_page() const;
        float usable_text_width() const;
        float line_height() const;
    };
}