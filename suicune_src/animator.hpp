#pragma once

#include <functional>
#include <memory>
#include <string>
#include "spritesheet.hpp"

namespace suicune
{
    class Animator
    {
    public:
        explicit Animator(std::shared_ptr<Spritesheet> spritesheet);

        void play_animation(const std::string &name);
        void play_animation(const std::string &name, std::function<void()> on_finished);

        void update(float dt);
        void draw_current_frame(float x, float y) const;

        bool is_finished() const;

        std::string get_current_animation() const;

        Spritesheet &get_spritesheet() const;

    private:
        std::shared_ptr<Spritesheet> sheet;

        std::string current_animation;
        float animation_timer = 0.0f;
        int current_frame_index = 0;

        bool finished = false;
        std::function<void()> finished_callback;
    };
}
