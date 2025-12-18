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
    };

    inline bool check_bound_box_collision(const BoundBox &a, const BoundBox &b, int slack = 0)
    {
        return (a.x < b.x + b.width - slack) &&
               (a.x + a.width > b.x + slack) &&
               (a.y < b.y + b.height - slack) &&
               (a.y + a.height > b.y + slack);
    }

    inline bool check_directional_bound_box_collision(
        const BoundBox &prev_a,
        const BoundBox &next_a,
        const BoundBox &b,
        Direction dir,
        int slack = 0)
    {
        // Next A edges
        int aL = next_a.x;
        int aR = next_a.x + next_a.width;
        int aT = next_a.y;
        int aB = next_a.y + next_a.height;

        // Prev A edges
        int paL = prev_a.x;
        int paR = prev_a.x + prev_a.width;
        int paT = prev_a.y;
        int paB = prev_a.y + prev_a.height;

        // B edges
        int bL = b.x;
        int bR = b.x + b.width;
        int bT = b.y;
        int bB = b.y + b.height;

        // Overlap on perpendicular axis (using next position)
        bool overlap_x = (aL < bR - slack) && (aR > bL + slack);
        bool overlap_y = (aT < bB - slack) && (aB > bT + slack);

        printf("Checking directional collision: overlap_x=%d, overlap_y=%d\n", overlap_x, overlap_y);

        switch (dir)
        {
        case RIGHT:
            // Were we left of B, and did our right edge cross into B's left edge?
            if (overlap_y && (paR <= bL) && (aR > bL))
            {
                printf("Collision detected moving RIGHT\n");
                return true;
            }
            break;

        case LEFT:
            // Were we right of B, and did our left edge cross into B's right edge?
            if (overlap_y && (paL >= bR) && (aL < bR))
            {
                printf("Collision detected moving LEFT\n");
                return true;
            }
            break;

        case DOWN:
            // Were we above B, and did our bottom edge cross into B's top edge?
            if (overlap_x && (paB <= bT) && (aB > bT))
            {
                printf("Collision detected moving DOWN\n");
                return true;
            }
            break;

        case UP:
            // Were we below B, and did our top edge cross into B's bottom edge?
            if (overlap_x && (paT >= bB) && (aT < bB))
            {
                printf("Collision detected moving UP\n");
                return true;
            }
            break;
        }

        return false;
    }
}
