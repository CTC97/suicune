#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "raylib.h"

namespace suicune
{
    struct Animation
    {
        std::vector<int> frame_indices;
        float frame_duration; // seconds per frame
        bool loop;
    };

    class Spritesheet
    {
    public:
        explicit Spritesheet(const char *file_path, int frame_width, int frame_height);
        ~Spritesheet();

        // disable copying.
        Spritesheet(const Spritesheet &) = delete;
        Spritesheet &operator=(const Spritesheet &) = delete;

        const Texture2D &get_texture() const;
        int get_frame_width() const;
        int get_frame_height() const;

        void draw_sprite(int frame_index, float x, float y) const;

        void define_animation(const std::string &name, const Animation &animation);
        const Animation *get_animation(const std::string &name) const;

    private:
        Texture2D texture{};
        int frame_width = 0;
        int frame_height = 0;

        std::unordered_map<std::string, Animation> animations;
    };
}
