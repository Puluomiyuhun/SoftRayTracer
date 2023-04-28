#include "sphere.h"

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    double a = r.direction().length_squared();
    double half_b = dot(r.direction(), oc);
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;
    if (discriminant > 0) {
        double root = sqrt(discriminant);
        double t = (-half_b - root) / a;
        if (t > t_min&& t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            vec3 uv;
            getSphereUV(outward_normal,uv);
            rec.u=uv.e[0];
            rec.v=uv.e[1];
            return true;
        }
        t = (-half_b + root) / a;
        if (t > t_min&& t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            vec3 uv;
            getSphereUV(outward_normal,uv);
            rec.u=uv.e[0];
            rec.v=uv.e[1];
            return true;
        }
    }
    return false;
}
