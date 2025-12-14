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

    void Spritesheet::draw_sprite(int frame_index, float global_x, float global_y)
    {
        int columns = texture.width / frame_width;
        int frame_x = (frame_index % columns) * frame_width;
        int frame_y = (frame_index / columns) * frame_height;

        Rectangle source = {static_cast<float>(frame_x), static_cast<float>(frame_y), static_cast<float>(frame_width), static_cast<float>(frame_height)};
        Rectangle dest = {global_x, global_y, static_cast<float>(frame_width), static_cast<float>(frame_height)};
        Vector2 origin = {0.0f, 0.0f};

        DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
    }

    void Spritesheet::define_animation(const std::string &name, const Animation &animation)
    {
        animations[name] = animation;
    }

    void Spritesheet::play_animation(const std::string &name)
    {
        if (name == current_animation)
            return;

        if (animations.find(name) != animations.end())
        {
            current_animation = name;
            animation_timer = 0.0f;
            current_frame_index = 0;
        }
    }

    void Spritesheet::update_animation(float dt)
    {
        if (animations.find(current_animation) == animations.end())
            return;

        const Animation &animation = animations[current_animation];
        animation_timer += dt;

        if (animation_timer >= animation.frame_duration)
        {
            animation_timer -= animation.frame_duration;
            current_frame_index++;

            if (current_frame_index >= animation.frame_indices.size())
            {
                if (animation.loop)
                {
                    current_frame_index = 0;
                }
                else
                {
                    current_frame_index = animation.frame_indices.size() - 1; // Stay on the last frame
                }
            }
        }
    }

    void Spritesheet::draw_current_frame(float x, float y)
    {
        if (animations.find(current_animation) == animations.end())
            return;

        const Animation &animation = animations[current_animation];
        int frame_index = animation.frame_indices[current_frame_index];
        draw_sprite(frame_index, x, y);
    }

    std::string Spritesheet::get_current_animation() const
    {
        return current_animation;
    }
}