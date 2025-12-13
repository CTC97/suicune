#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "raylib.h"

namespace barley
{

    struct Animation
    {
        std::vector<int> frame_indices;
        float frame_duration; // Duration of each frame in seconds
        bool loop;
    };

    class Spritesheet
    {
    public:
        explicit Spritesheet(const char *file_path, int frame_width, int frame_height);
        ~Spritesheet();

        Texture2D &get_texture();
        int get_frame_width();
        int get_frame_height();

        void draw_sprite(int frame_index, float x, float y);

        void define_animation(const std::string &name, const Animation &animation);
        void play_animation(const std::string &name);
        void update_animation(float dt);
        void draw_current_frame(float x, float y);

    private:
        Texture2D texture;
        int frame_width;
        int frame_height;

        std::unordered_map<std::string, Animation> animations;
        std::string current_animation;
        float animation_timer = 0.0f;
        int current_frame_index = 0;
    };
}