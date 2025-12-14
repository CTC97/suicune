#include "dialog_manager.hpp"

namespace barley
{
    DialogManager::DialogManager()
        : current_node_index(-1), active(false), text_speed(0.05f), text_timer(0.0f), char_index(0)
    {
    }

    DialogManager::~DialogManager()
    {
    }

    void DialogManager::start_dialog(const std::vector<DialogNode> &nodes)
    {
        dialog_nodes = nodes;
        current_node_index = 0;
        active = true;
        current_text.clear();
        char_index = 0;
        text_timer = 0.0f;
    }

    void DialogManager::update(float dt)
    {
        if (!active)
            return;

        // Handle text animation
        const DialogNode &current_node = dialog_nodes[current_node_index];
        printf("Current node text: %s\n", current_node.text.c_str());
        if (char_index < static_cast<int>(current_node.text.size()))
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                // Skip the typewriter effect and display the full text
                current_text = current_node.text;
                char_index = static_cast<int>(current_node.text.size());
            }
            else
            {
                // Continue the typewriter effect
                text_timer += dt;
                if (text_timer >= text_speed)
                {
                    text_timer = 0.0f;
                    current_text += current_node.text[char_index];
                    char_index++;
                }
            }
        }
        else
        {
            // Wait for user input to progress the dialog
            if (IsKeyPressed(KEY_ENTER)) // Example: Enter key to progress
            {
                step_dialog();
            }
        }
    }

    void DialogManager::draw(float base_x, float base_y)
    {
        if (!active)
            return;

        // Draw dialog box background (placeholder rendering)
        if (dialog_box_texture.id != 0) // Check if the texture is valid
        {
            DrawTexture(dialog_box_texture, base_x, base_y, WHITE); // Example position and color
        }
        else
        {
            throw std::runtime_error("The dialog manager tried to draw but the dialog box texture could not be found.");
        }

        // Draw the current text (placeholder rendering)
        printf("Drawing current text: %s\n", current_text.c_str());
        DrawText(current_text.c_str(), base_x + text_padding_x, base_y + text_padding_y, 20, WHITE);

        // Optionally, draw options if available
        const DialogNode &current_node = dialog_nodes[current_node_index];
        if (!current_node.options.empty())
        {
            int y_offset = 450;
            for (const auto &option : current_node.options)
            {
                DrawText(option.label.c_str(), base_x + text_padding_x, base_y + y_offset + text_padding_y, 20, GRAY);
                y_offset += 30;
            }
        }
    }

    bool DialogManager::is_active() const
    {
        return active;
    }

    void DialogManager::step_dialog()
    {
        if (current_node_index < 0 || current_node_index >= static_cast<int>(dialog_nodes.size()))
        {
            active = false; // End the dialog
            return;
        }

        const DialogNode &current_node = dialog_nodes[current_node_index];

        // If there are options, wait for user input to select one
        if (!current_node.options.empty())
        {
            // For now, we’ll assume the first option is selected automatically
            current_node_index = current_node.options[0].next_node_index;
        }
        else
        {
            // Move to the next node
            current_node_index++;
        }

        // If we've reached the end of the dialog
        if (current_node_index < 0 || current_node_index >= static_cast<int>(dialog_nodes.size()))
        {
            active = false;
        }
        else
        {
            // Reset text for the next node
            current_text.clear();
            char_index = 0;
        }
    }

    const DialogNode *DialogManager::get_current_node() const
    {
        if (current_node_index >= 0 && current_node_index < static_cast<int>(dialog_nodes.size()))
        {
            return &dialog_nodes[current_node_index];
        }
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

    void DialogManager::set_text_padding(int padding_x, int padding_y)
    {
        text_padding_x = padding_x;
        text_padding_y = padding_y;
    }
}