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
        int offset_x;
        int offset_y;
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
}
