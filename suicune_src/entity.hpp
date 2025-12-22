// x, y, animator, width, height
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "raylib.h"
#include "animator.hpp"
#include "dialog_manager.hpp"
#include "util.hpp"

namespace suicune
{
    class Entity
    {
    public:
        Entity(std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y);
        virtual ~Entity() = default;

        virtual void update(float dt);
        virtual void draw();

        void play_animation(const std::string &animation_name);
        void play_animation(const std::string &animation_name, std::function<void()> on_finished);
        std::string get_current_animation() const;

        Vector2 get_position() const;

        float get_width();
        float get_height();

        void set_solid(bool solid);
        bool is_solid() const;

        virtual void interact();
        void set_interaction_callback(std::function<void()> callback);

        virtual void collide();
        void set_collision_callback(std::function<void()> callback);

        void set_dialog_manager(DialogManager *dialog_manager);
        void set_dialog(const std::vector<DialogNode> &dialog);

        void set_bound_box_position(float x, float y);
        void set_bound_box_dimensions(int width, int height);
        void set_bound_box_offset(float offset_x, float offset_y);
        BoundBox get_bound_box() const;

        Direction get_current_direction();
        void set_stop_movement(bool stop);
        bool is_movement_stopped() const;

        Spritesheet &get_spritesheet() const;

        void tween_to(Vector2 target, float duration, bool stop_movement = true);
        bool is_tweening() const;
        void cancel_tween();

        void set_shader(Shader shader);
        void clear_shader();

        bool is_alive() const;
        void destroy();

    protected:
        Animator animator;

        int width;
        int height;
        float x;
        float y;
        bool solid = true;

        bool alive = true;

        BoundBox bound_box{0, 0, 0, 0, 0, 0};

        Direction current_direction;
        bool stop_movement = false;

        DialogManager *dm = nullptr; // pointer to the shared DialogManager
        std::vector<DialogNode> dialog_sequence;

        std::function<void()> interaction_callback;
        std::function<void()> collision_callback;

        Tween tween;
    };
}
