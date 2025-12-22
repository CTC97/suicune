#include "animator.hpp"

namespace suicune
{
    Animator::Animator(std::shared_ptr<Spritesheet> spritesheet)
        : sheet(std::move(spritesheet))
    {
    }

    void Animator::play_animation(const std::string &name)
    {
        play_animation(name, nullptr);
    }

    void Animator::play_animation(const std::string &name, std::function<void()> on_finished)
    {
        if (!sheet || !sheet->get_animation(name) || name == current_animation)
            return;

        current_animation = name;
        animation_timer = 0.0f;
        current_frame_index = 0;

        finished = false;
        finished_callback = std::move(on_finished);
    }

    void Animator::update(float dt)
    {
        if (!sheet || finished)
            return;

        const Animation *anim = sheet->get_animation(current_animation);
        if (!anim)
            return;

        animation_timer += dt;

        while (animation_timer >= anim->frame_duration)
        {
            animation_timer -= anim->frame_duration;
            current_frame_index++;

            const int last = (int)anim->frame_indices.size() - 1;
            if (current_frame_index > last)
            {
                if (anim->loop)
                {
                    current_frame_index = 0;
                }
                else
                {
                    current_frame_index = last;
                    finished = true;

                    // Fire once
                    if (finished_callback)
                    {
                        auto fire_callback = std::move(finished_callback);
                        finished_callback = nullptr;
                        fire_callback();
                    }
                    return;
                }
            }
        }
    }

    void Animator::draw_current_frame(float x, float y) const
    {
        if (!sheet)
            return;

        const Animation *anim = sheet->get_animation(current_animation);
        if (!anim || anim->frame_indices.empty())
            return;

        int frame_index = anim->frame_indices[current_frame_index];

        sheet->draw_sprite(frame_index, x, y);

                if (shader)
        {
            TraceLog(LOG_INFO, "Drawing with shader");
            float t = GetTime();
            int timeLoc = GetShaderLocation(*shader, "time");
            SetShaderValue(*shader, timeLoc, &t, SHADER_UNIFORM_FLOAT);
            Vector2 res = {(float)sheet->get_frame_width(), (float)sheet->get_frame_height()};
            int resLoc = GetShaderLocation(*shader, "resolution");
            SetShaderValue(*shader, resLoc, &res, SHADER_UNIFORM_VEC2);
            BeginShaderMode(*shader);
        }

        sheet->draw_sprite(frame_index, x, y);

        if (shader)
        {
            EndShaderMode();
        }
    }

    std::string Animator::get_current_animation() const
    {
        return current_animation;
    }

    bool Animator::is_finished() const
    {
        return finished;
    }

    Spritesheet &Animator::get_spritesheet() const
    {
        return *sheet;
    }

    void Animator::set_shader(std::shared_ptr<Shader> shader)
    {
        this->shader = shader;
    }

    void Animator::clear_shader()
    {
        shader.reset();
    }
}
