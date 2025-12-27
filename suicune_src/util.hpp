#pragma once
#include <cstdio>
#include <functional>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

namespace suicune
{
    // <----------------- Movement ----------------->
    enum Direction
    {
        UP,
        UP_RIGHT,
        UP_LEFT,
        DOWN,
        DOWN_RIGHT,
        DOWN_LEFT,
        LEFT,
        RIGHT
    };

    // <----------------- Bounding ----------------->
    struct BoundBox
    {
        float x;
        float y;
        int width;
        int height;
        float offset_x = 0;
        float offset_y = 0;
    };

    inline bool check_bound_box_collision(const BoundBox &a, const BoundBox &b, float slack = 0, int offset_x = 0, int offset_y = 0)
    {
        return (a.x + offset_x < b.x + b.width - slack) &&
               (a.x + a.width + offset_x > b.x + slack) &&
               (a.y + offset_y < b.y + b.height - slack) &&
               (a.y + a.height + offset_y > b.y + slack);
    }

    inline bool check_directional_bound_box_collision(
        const BoundBox &prev_a,
        const BoundBox &next_a,
        const BoundBox &b,
        Direction dir,
        int slack = 0,
        float offset_x = 0,
        float offset_y = 0)
    {
        float aL = next_a.x + offset_x, aR = next_a.x + next_a.width + offset_x;
        float aT = next_a.y + offset_y, aB = next_a.y + next_a.height + offset_y;

        float paL = prev_a.x + offset_x, paR = prev_a.x + prev_a.width + offset_x;
        float paT = prev_a.y + offset_y, paB = prev_a.y + prev_a.height + offset_y;

        float bL = b.x, bR = b.x + b.width;
        float bT = b.y, bB = b.y + b.height;

        bool overlap_x = (aL < bR - slack) && (aR > bL + slack);
        bool overlap_y = (aT < bB - slack) && (aB > bT + slack);

        // Cardinal collision checks (your existing logic)
        auto hit_right = [&]()
        { return overlap_y && (paR <= bL) && (aR > bL); };
        auto hit_left = [&]()
        { return overlap_y && (paL >= bR) && (aL < bR); };
        auto hit_down = [&]()
        { return overlap_x && (paB <= bT) && (aB > bT); };
        auto hit_up = [&]()
        { return overlap_x && (paT >= bB) && (aT < bB); };

        // Optional: only consider components we actually moved in
        float dx = (next_a.x - prev_a.x);
        float dy = (next_a.y - prev_a.y);

        switch (dir)
        {
        case RIGHT:
            return hit_right();
        case LEFT:
            return hit_left();
        case DOWN:
            return hit_down();
        case UP:
            return hit_up();

        case UP_RIGHT:
            return ((dx > 0.0f) && hit_right()) || ((dy < 0.0f) && hit_up());
        case UP_LEFT:
            return ((dx < 0.0f) && hit_left()) || ((dy < 0.0f) && hit_up());
        case DOWN_RIGHT:
            return ((dx > 0.0f) && hit_right()) || ((dy > 0.0f) && hit_down());
        case DOWN_LEFT:
            return ((dx < 0.0f) && hit_left()) || ((dy > 0.0f) && hit_down());
        }

        return false;
    }

    inline float clampf(float v, float lo, float hi)
    {
        if (v < lo)
            return lo;
        if (v > hi)
            return hi;
        return v;
    }

    // Distance from point p to axis-aligned rectangle r (0 if inside)
    inline float point_rect_distance(Vector2 p, BoundBox &r)
    {
        float rx = r.x;
        float ry = r.y;
        float rw = (float)r.width;
        float rh = (float)r.height;

        float cx = clampf(p.x, rx, rx + rw);
        float cy = clampf(p.y, ry, ry + rh);

        float dx = p.x - cx;
        float dy = p.y - cy;
        return sqrtf(dx * dx + dy * dy);
    }

    inline Vector2 bound_box_center(const BoundBox &b)
    {
        return Vector2{b.x + b.width / 2.0f, b.y + b.height / 2.0f};
    }

    // <----------------- Tween ----------------->
    struct Tween
    {
        bool active = false;
        Vector2 start{0, 0};
        Vector2 target{0, 0};
        float duration = 0.0f;
        float elapsed = 0.0f;
        bool stop_movement = true;
        std::function<void()> on_finished = nullptr;
    };

    inline float lerpf(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    inline Vector2 lerp_v2(Vector2 a, Vector2 b, float t)
    {
        return Vector2{lerpf(a.x, b.x, t), lerpf(a.y, b.y, t)};
    }

    // Returns true while tween is still running; false when finished (or not active)
    inline bool step_tween(Tween &tw, float dt, Vector2 &out_pos)
    {
        if (!tw.active)
            return false;

        // handle instant or invalid durations gracefully
        if (tw.duration <= 0.0f)
        {
            out_pos = tw.target;
            tw.active = false;
            return false;
        }

        tw.elapsed += dt;

        float t = tw.elapsed / tw.duration;
        if (t >= 1.0f)
        {
            t = 1.0f;
            tw.active = false;
            if (tw.on_finished)
            {
                tw.on_finished();
                tw.on_finished = nullptr;
            }
        }

        out_pos = lerp_v2(tw.start, tw.target, t);
        return tw.active;
    }

    // <----------------- Shake ----------------->
    struct Shake
    {
        float timer = 0.0f;
        float strength = 0.0f;
        bool is_alive = false;
    };

    // <----------------- File Loading ----------------->
    inline void set_cwd_to_app_resources_if_present()
    {
        const char *exe_dir = GetApplicationDirectory(); // .../Contents/MacOS when bundled

        if (!exe_dir)
            return;

        // If we're in a macOS .app, this folder should exist:
        //   <App>.app/Contents/Resources
        const char *resources_dir = TextFormat("%s/../Resources", exe_dir);

        if (DirectoryExists(resources_dir))
        {
            ChangeDirectory(exe_dir);
            ChangeDirectory("../Resources");
            TraceLog(LOG_INFO, "Changed CWD to bundle Resources: %s", GetWorkingDirectory());
        }
        else
        {
            TraceLog(LOG_INFO, "Not a bundle run; keeping CWD: %s", GetWorkingDirectory());
        }
    }
}
