#include "spritesheet.hpp"

namespace suicune
{
    Spritesheet::Spritesheet(const char *file_path, int frame_width, int frame_height)
        : frame_width(frame_width), frame_height(frame_height)
    {
        texture = LoadTexture(file_path);
        if (texture.id == 0)
        {
            throw std::runtime_error(std::string("Failed to load texture from file: ") + file_path);
        }
    }

    Spritesheet::~Spritesheet()
    {
        if (texture.id != 0)
            UnloadTexture(texture);
    }

    const Texture2D &Spritesheet::get_texture() const { return texture; }
    int Spritesheet::get_frame_width() const { return frame_width; }
    int Spritesheet::get_frame_height() const { return frame_height; }

    void Spritesheet::draw_sprite(int frame_index, float global_x, float global_y) const
    {
        int columns = texture.width / frame_width;
        int frame_x = (frame_index % columns) * frame_width;
        int frame_y = (frame_index / columns) * frame_height;

        Rectangle source = {(float)frame_x, (float)frame_y, (float)frame_width, (float)frame_height};
        Rectangle dest = {global_x, global_y, (float)frame_width, (float)frame_height};
        Vector2 origin = {0.0f, 0.0f};

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    }

    void Spritesheet::define_animation(const std::string &name, const Animation &animation)
    {
        animations[name] = animation;
    }

    const Animation *Spritesheet::get_animation(const std::string &name) const
    {
        auto it = animations.find(name);
        if (it == animations.end())
            return nullptr;
        return &it->second;
    }
}
