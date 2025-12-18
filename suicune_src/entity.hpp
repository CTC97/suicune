// x, y, spritesheet, width, height
#pragma once

#include "raylib.h"
#include "spritesheet.hpp"
#include "dialog_manager.hpp"
#include "util.hpp"

namespace suicune
{

    class Entity
    {
    public:
        Entity(Spritesheet &spritesheet, int width, int height, int x, int y, bool solid = true);
        virtual ~Entity() = default;

        virtual void update(float dt);
        virtual void draw();

        void play_animation(const std::string &animation_name);
        std::string get_current_animation() const;

        Vector2 get_position() const;

        int get_width();
        int get_height();

        void set_solid(bool solid);
        bool is_solid() const;

        virtual void interact();

        void set_dialog_manager(DialogManager *dialog_manager);
        void set_dialog(const std::vector<DialogNode> &dialog);

        void set_bound_dimensions(int width, int height);
        BoundBox get_bound_box() const;

    protected:
        Spritesheet &spritesheet;
        int width;
        int height;
        int x;
        int y;
        bool solid;

        BoundBox bound_box{0, 0, 0, 0};

        DialogManager *dm = nullptr;             // Pointer to the shared DialogManager
        std::vector<DialogNode> dialog_sequence; // Entity-specific dialog
    };
}