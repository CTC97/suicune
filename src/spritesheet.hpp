#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include "raylib.h"

namespace barley
{

    class Spritesheet
    {
    public:
        explicit Spritesheet(const char *file_path, int frame_width, int frame_height);
        ~Spritesheet();

        Texture2D &get_texture();
        int get_frame_width();
        int get_frame_height();

        void draw_sprite(int frame_index, float x, float y);

    private:
        Texture2D texture;
        int frame_width;
        int frame_height;
    };
}