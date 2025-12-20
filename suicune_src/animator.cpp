#include "animator.hpp"

namespace suicune
{
    Animator::Animator(std::shared_ptr<Spritesheet> spritesheet)
        : sheet(std::move(spritesheet))
    {
    }

    void Animator::play_animation(const std::string &name)
    {
        if (name == current_animation)
            return;

        if (sheet && sheet->get_animation(name))
        {
            current_animation = name;
            animation_timer = 0.0f;
            current_frame_index = 0;
        }
    }

    void Animator::update(float dt)
    {
        if (!sheet)
            return;

        const Animation *anim = sheet->get_animation(current_animation);
        if (!anim)
            return;

        animation_timer += dt;

        if (animation_timer >= anim->frame_duration)
        {
            animation_timer -= anim->frame_duration;
            current_frame_index++;

            if (current_frame_index >= (int)anim->frame_indices.size())
            {
                if (anim->loop)
                    current_frame_index = 0;
                else
                    current_frame_index = (int)anim->frame_indices.size() - 1;
            }
        }
    }

    void Animator::draw_current_frame(float x, float y) const
    {
        if (!sheet)
            return;

        const Animation *anim = sheet->get_animation(current_animation);
        if (!anim)
            return;

        int frame_index = anim->frame_indices[current_frame_index];
        sheet->draw_sprite(frame_index, x, y);
    }

    std::string Animator::get_current_animation() const
    {
        return current_animation;
    }

    Spritesheet &Animator::get_spritesheet() const
    {
        return *sheet;
    }
}
