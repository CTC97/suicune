#pragma once

#include <functional>
#include "raylib.h"
#include "scene.hpp"

namespace suicune
{
    class Interactable
    {
    public:
        Interactable(Scene *scene, int width, int height, float x, float y);

        void update(int width, int height, float x, float y);

        void set_clickable(bool clickable);
        bool is_clickable() const;
        void set_clicked_callback(std::function<void()> callback);

        void set_hoverable(bool hoverable);
        bool is_hoverable() const;
        void set_hovered_callback(std::function<void()> callback);
        bool is_hovered_over() const;
        void set_unhovered_callback(std::function<void()> callback);

    private:
        Scene *scene;
        int width;
        int height;
        float x;
        float y;

        std::function<void()> clicked_callback;
        std::function<void()> hovered_callback;
        std::function<void()> unhovered_callback;

        bool clickable = false;
        bool hoverable = false;
        bool hovered_over = false;
    };
}