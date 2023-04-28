#include"bvh_node.h"

inline int _x_cmp(const void * lhs, const void * rhs)
{
    hittable * lc = *(hittable**)lhs;
    hittable * rc = *(hittable**)rhs;
    aabb lbox = lc->getbox();
    aabb rbox = rc->getbox();
    if (lbox.min().x() - rbox.min().x() < 0.)
        return -1;
    else
        return 1;
}

inline int _y_cmp(const void * lhs, const void * rhs)
{
    hittable * lc = *(hittable**)lhs;
    hittable * rc = *(hittable**)rhs;
    aabb lbox = lc->getbox();
    aabb rbox = rc->getbox();
    if (lbox.min().y() - rbox.min().y() < 0.)
        return -1;
    else
        return 1;
}

static int times=0;
inline int _z_cmp(const void * lhs, const void * rhs)
{
    printf("%d\n",times++);fflush(stdout);
    hittable * lc = *(hittable**)lhs;
    hittable * rc = *(hittable**)rhs;
    aabb lbox = lc->getbox();
    aabb rbox = rc->getbox();
    if (lbox.min().z() - rbox.min().z() < 0.)
        return -1;
    else
        return 1;
}

bvh_node::bvh_node(hittable** world, const int n)
{
    /*int axis = static_cast<int>(3 * random_double());
    if (axis == 0)
        qsort(world, n, sizeof(hittable*), _x_cmp);
    else if (axis == 1)
        qsort(world, n, sizeof(hittable*), _y_cmp);
    else
        qsort(world, n, sizeof(hittable*), _z_cmp);
    qsort(world, n, sizeof(hittable*), _z_cmp);*/
    if (n == 1)
        _left = _right = world[0];
    else if (n == 2)
        _left = world[0],
        _right = world[1];
    else
        _left = new bvh_node(world, n / 2),
        _right = new bvh_node(world + n / 2, n - n / 2);

    aabb lbox = _left->getbox();
    aabb rbox = _right->getbox();

    _box = aabb::_surrounding_box(lbox, rbox);
}

bool bvh_node::hit(const ray& sight, double t_min, double t_max, hit_record& info)const
{
    if (_box.hit(sight, t_min, t_max))
    {
        hit_record linfo, rinfo;
        bool lhit = _left->hit(sight, t_min, t_max, linfo);
        bool rhit = _right->hit(sight, t_min, t_max, rinfo);
        if (lhit && rhit)
        {
            if (linfo.t < rinfo.t)
                info = linfo;
            else
                info = rinfo;
            return true;
        }
        else if (lhit)
        {
            info = linfo;
            return true;
        }
        else if (rhit)
        {
            info = rinfo;
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
