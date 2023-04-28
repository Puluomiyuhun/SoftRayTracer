#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include"hittable.h"
#include<memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;
using std::vector;

class hittable_list :public hittable {
public:
    hittable** objects = new hittable*[30000];
    int count = 0;
public:
    hittable_list(){ }
    hittable_list(hittable* object) { add(object); }

    void add(hittable* object) { objects[count++]=object; }

    aabb getbox() const override{
        return aabb(vec3(9999,9999,9999),vec3(9999,9999,9999));
    }
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
};

#endif // !HITTABLE_LIST_H
