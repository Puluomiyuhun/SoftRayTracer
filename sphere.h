#ifndef SPHERE_H
#define SPHERE_H

#include"hittable.h"
#include"vec3.h"
#include"aabb.h"

class sphere :public hittable {
public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
public:
    sphere() { }
    sphere(point3 cen, double r, shared_ptr<material> mat) :center(cen), radius(r),mat_ptr(mat) { }
    void getSphereUV(vec3 &p, vec3 &tex)const
    {
        float phi = atan2(p.z(), p.x());
        float theta = asin(p.y());
        tex.e[0] = 1 - (phi + M_PI) / (2*M_PI);
        tex.e[1] = (theta + M_PI/2) / M_PI;
    }
    aabb getbox() const override{
        return aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    }
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

#endif // ! SPHERE_H


