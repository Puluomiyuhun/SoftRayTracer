#ifndef BVH_NODE_H
#define BVH_NODE_H
#include"hittable.h"
#include"aabb.h"

class bvh_node :public hittable
{
public:
    bvh_node(){}
    bvh_node(hittable** world, const int n);
    virtual bool hit(const ray& sight, double t_min, double t_max, hit_record& info)const override;
    virtual aabb getbox()const override{
        return _box;
    }
private:
    hittable* _left;
    hittable* _right;
    aabb _box;
};

#endif // BVH_NODE_H
