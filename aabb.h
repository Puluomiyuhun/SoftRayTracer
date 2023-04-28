#ifndef AABB_H
#define AABB_H
#include"rtweekend.h"
#include"vec3.h"
#include"ray.h"

class aabb
{
public:
    aabb(){}
    aabb(const vec3& a, const vec3& b);
    inline bool hit(const ray& sight, double tmin, double tmax)const;
    static aabb _surrounding_box(aabb box1, aabb box2)
    {
        auto fmin = [](const double a, const double b) {return a < b ? a : b; };
        auto fmax = [](const double a, const double b) {return a > b ? a : b; };
        vec3 min{    fmin(box1.min().x(),box2.min().x()),
                    fmin(box1.min().y(),box2.min().y()),
                    fmin(box1.min().z(),box2.min().z()) };
        vec3 max{    fmax(box1.max().x(),box2.max().x()),
                    fmax(box1.max().y(),box2.max().y()),
                    fmax(box1.max().z(),box2.max().z()) };
        return aabb(min, max);
    }

public:
    inline vec3 min()const { return _min; }
    inline vec3 max()const { return _max; }

private:
    vec3 _min;
    vec3 _max;
};

inline aabb::aabb(const vec3& a, const vec3& b):_min(a), _max(b){}

bool aabb::hit(const ray& sight, double tmin, double tmax)const
{
    /*for (int i = 0; i < 3; ++i)
    {
        double div = 1.0 / sight.direction()[i];
        double t1 = (_min[i] - sight.origin()[i]) / sight.direction()[i];
        double t2 = (_max[i] - sight.origin()[i]) / sight.direction()[i];
        if (div < 0.0f)swap(t1, t2);
        if (mind(t2, tmax) <= maxd(t1, tmin))
            return false;
    }
    if(times==0)
    {
        printf("ori: %f %f %f\n",sight.origin().x(),sight.origin().y(),sight.origin().z());
        printf("dir: %f %f %f\n",sight.direction().x(),sight.direction().y(),sight.direction().z());
        printf("min: %f %f %f\n",min().x(),min().y(),min().z());
        printf("maxd: %f %f %f\n",max().x(),max().y(),max().z());
        times++;
    }
    return true;*/
    double ox = sight.origin().x();double oy = sight.origin().y();double oz = sight.origin().z();
    double dx = sight.direction().x();double dy = sight.direction().y();double dz = sight.direction().z();
    double tx_min,ty_min,tz_min;
    double tx_max,ty_max,tz_max;

    //x0,y0,z0为包围体的最小顶点
    //x1,y1,z1为包围体的最大顶点
    double x0=min().x(),y0=min().y(),z0=min().z();
    double x1=max().x(),y1=max().y(),z1=max().z();
    if(fabs(dx) < 0.000001f)
    {
        //若射线方向矢量的x轴分量为0且原点不在盒体内
        if(ox < x1 || ox > x0)
            return false ;
    }
    else
    {
        if(dx>=0)
        {
            tx_min = (x0-ox)/dx;
            tx_max = (x1-ox)/dx;
        }
        else
        {
            tx_min = (x1-ox)/dx;
            tx_max = (x0-ox)/dx;
        }
    }

    if(fabs(dy) < 0.000001f)
    {
        //若射线方向矢量的x轴分量为0且原点不在盒体内
        if(oy < y1 || oy > y0)
            return false ;
    }
    else
    {
        if(dy>=0)
        {
            ty_min = (y0-oy)/dy;
            ty_max = (y1-oy)/dy;
        }
        else
        {
            ty_min = (y1-oy)/dy;
            ty_max = (y0-oy)/dy;
        }

    }

    if(fabs(dz) < 0.000001f)
    {
        //若射线方向矢量的x轴分量为0且原点不在盒体内
        if(oz < z1 || oz > z0)
            return false ;
    }
    else
    {
        if(dz>=0)
        {
            tz_min = (z0-oz)/dz;
            tz_max = (z1-oz)/dz;
        }
        else
        {
            tz_min = (z1-oz)/dz;
            tz_max = (z0-oz)/dz;
        }

    }

    double t0,t1;

    //光线进入平面处（最靠近的平面）的最大t值
    t0=maxd(tz_min,maxd(tx_min,ty_min));

    //光线离开平面处（最远离的平面）的最小t值
    t1=mind(tz_max,mind(tx_max,ty_max));

    return t0<t1;
}

#endif // AABB_H
