#ifndef HITTABLE_H
#define	HITTABLE_H

#include"ray.h"
#include"aabb.h"

class material;

struct hit_record {
    point3 p;   //交点
    vec3 normal;	//法向量
    shared_ptr<material> mat_ptr;
    double t;	//距离
    bool front_face;
    double u,v;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual aabb getbox() const = 0;
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
#endif // ! HITTABLE_H
