// x, y, animator, width, height
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "raylib.h"
#include "animator.hpp"
#include "dialog_manager.hpp"
#include "util.hpp"
#include "scene.hpp"

namespace suicune
{
    class Entity
    {
    public:
        Entity(Scene *scene, std::shared_ptr<Spritesheet> spritesheet, int width, int height, float x, float y);
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

        void tween_to(Vector2 target, float duration);
        void tween_to(Vector2 target, float duration, std::function<void()> on_finished);
        bool is_tweening() const;
        void cancel_tween();

        void set_shader(std::shared_ptr<Shader> shader);
        void clear_shader();

        bool is_alive() const;
        void destroy();

        void set_clickable(bool clickable);
        bool is_clickable() const;
        void set_clicked_callback(std::function<void()> callback);

        void set_hoverable(bool hoverable);
        bool is_hoverable() const;
        void set_hovered_callback(std::function<void()> callback);
        bool is_hovered_over() const;
        void set_unhovered_callback(std::function<void()> callback);

    protected:
        std::string type = "";
        Scene *scene;
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
        std::function<void()> clicked_callback;
        std::function<void()> hovered_callback;
        std::function<void()> unhovered_callback;

        Tween tween;

        bool clickable = false;
        bool hoverable = false;
        bool hovered_over = false;
    };
}
