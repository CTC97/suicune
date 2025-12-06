#include "spritesheet.hpp"

namespace barley
{

    Spritesheet::Spritesheet(const char *file_path, int frame_width, int frame_height)
        : frame_width(frame_width), frame_height(frame_height)
    {
        texture = LoadTexture(file_path);
        if (texture.id == 0)
        {
            printf("Failed to load texture from file: "
                   "%s\n",
                   file_path);
        }
    }

    Spritesheet::~Spritesheet()
    {
        UnloadTexture(texture);
    }

    Texture2D &Spritesheet::get_texture()
    {
        return texture;
    }

    int Spritesheet::get_frame_width()
    {
        return frame_width;
    }

    int Spritesheet::get_frame_height()
    {
        return frame_height;
    }

    void Spritesheet::draw_sprite(int frame_index, float x, float y)
    {
        int columns = texture.width / frame_width;
        int frame_x = (frame_index % columns) * frame_width;
        int frame_y = (frame_index / columns) * frame_height;

        Rectangle source = {static_cast<float>(frame_x), static_cast<float>(frame_y), static_cast<float>(frame_width), static_cast<float>(frame_height)};
        Rectangle dest = {x * frame_width, y * frame_height, static_cast<float>(frame_width), static_cast<float>(frame_height)};
        Vector2 origin = {0.0f, 0.0f};

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    }
}