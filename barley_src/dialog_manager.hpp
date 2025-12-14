#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "raylib.h"

namespace barley
{
    struct DialogOption
    {
        std::string label;   // Text for the option
        int next_node_index; // Index of the next dialog node (-1 for end)
    };

    struct DialogNode
    {
        std::string text;                  // Dialog text
        std::string speaker;               // Name of the speaker
        std::vector<DialogOption> options; // Optional branching options
    };

    class DialogManager
    {
    public:
        DialogManager();
        ~DialogManager();

        void start_dialog(const std::vector<DialogNode> &nodes);
        void update(float dt);
        void draw();
        bool is_active() const;

        const DialogNode *get_current_node() const;

    private:
        void step_dialog();

        // State
        std::vector<DialogNode> dialog_nodes; // Current dialog sequence
        int current_node_index;               // Index of the current dialog node
        bool active;                          // Whether a dialog is active

        // Rendering-related
        std::string current_text; // Text currently displayed on screen

        float text_speed; // Speed of text reveal
        float text_timer; // Timer for text animation
        int char_index;   // Index of the next character to display
    };
}