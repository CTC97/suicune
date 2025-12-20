#pragma once
#include <cstdio>

namespace suicune
{
    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct BoundBox
    {
        int x;
        int y;
        int width;
        int height;
        int offset_x = 0;
        int offset_y = 0;
    };

    inline bool check_bound_box_collision(const BoundBox &a, const BoundBox &b, int slack = 0, int offset_x = 0, int offset_y = 0)
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
        int offset_x = 0,
        int offset_y = 0)
    {
        int aL = next_a.x + offset_x, aR = next_a.x + next_a.width + offset_x;
        int aT = next_a.y + offset_y, aB = next_a.y + next_a.height + offset_y;

        int paL = prev_a.x + offset_x, paR = prev_a.x + prev_a.width + offset_x;
        int paT = prev_a.y + offset_y, paB = prev_a.y + prev_a.height + offset_y;

        int bL = b.x, bR = b.x + b.width;
        int bT = b.y, bB = b.y + b.height;

        bool overlap_x = (aL < bR - slack) && (aR > bL + slack);
        bool overlap_y = (aT < bB - slack) && (aB > bT + slack);

        switch (dir)
        {
        case RIGHT:
            return overlap_y && (paR <= bL) && (aR > bL);
        case LEFT:
            return overlap_y && (paL >= bR) && (aL < bR);
        case DOWN:
            return overlap_x && (paB <= bT) && (aB > bT);
        case UP:
            return overlap_x && (paT >= bB) && (aT < bB);
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
        float rx = (float)r.x;
        float ry = (float)r.y;
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
}
