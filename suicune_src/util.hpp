namespace suicune
{

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
}