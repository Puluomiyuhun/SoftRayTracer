#ifndef TRIANGLE_H
#define TRIANGLE_H


#include"hittable.h"
#include"vec3.h"
#include"aabb.h"

class triangle :public hittable {
public:
    point3 pt[3];
    double u[3];
    double v[3];
    vec3 normal;
    shared_ptr<material> mat_ptr;
public:
    triangle() { }
    triangle(point3 a, point3 b, point3 c, vec3 n,shared_ptr<material> mat):
        pt{a,b,c},mat_ptr(mat)
    {
        /*vec3 v1=(pt[0]-pt[1]);
        vec3 v2=(pt[1]-pt[2]);
        normal=cross(v1,v2);
        normal/=normal.length();*/
        normal=n/n.length();
    }
    triangle(point3 a, point3 b, point3 c, vec3 n, shared_ptr<material> mat,
             double u1,double v1,double u2,double v2,double u3,double v3):
        pt{a,b,c},u{u1,u2,u3},v{v1,v2,v3},mat_ptr(mat)
    {
        //printf("%lf %lf %lf\n",(pt[1]-pt[0]).x(),(pt[1]-pt[0]).y(),(pt[1]-pt[0]).z());
        normal=n/n.length();
    }
    aabb getbox() const override{
        vec3 small(
                mind(pt[0].x(),mind(pt[1].x(),pt[2].x()))-0.00001,
                mind(pt[0].y(),mind(pt[1].y(),pt[2].y()))-0.00001,
                mind(pt[0].z(),mind(pt[1].z(),pt[2].z()))-0.00001
                    );
        vec3 big(
                maxd(pt[0].x(),maxd(pt[1].x(),pt[2].x())),
                maxd(pt[0].y(),maxd(pt[1].y(),pt[2].y())),
                maxd(pt[0].z(),maxd(pt[1].z(),pt[2].z()))
                    );
        return aabb(small,big);
    }
    bool SameSide(vec3 A, vec3 B, vec3 C, vec3 P)const;
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

class cube
{
public:
    triangle* tg[120];
    int faceCnt=12;
    shared_ptr<material> mat_ptr;
public:
    cube(){}
    cube(point3 pos,double size,shared_ptr<material> mat):mat_ptr(mat)
    {
        double half=size/2;
        /*前面*/
        tg[0]=new triangle(pos+vec3(-half,half,half),pos+vec3(half,half,half),
                       pos+vec3(-half,-half,half),vec3(0,0,1),mat_ptr);
        tg[1]=new triangle(pos+vec3(half,-half,half),pos+vec3(half,half,half),
                       pos+vec3(-half,-half,half),vec3(0,0,1),mat_ptr);
        /*上面*/
        tg[2]=new triangle(pos+vec3(-half,half,-half),pos+vec3(half,half,-half),
                       pos+vec3(-half,half,half),vec3(0,1,0),mat_ptr);
        tg[3]=new triangle(pos+vec3(half,half,half),pos+vec3(half,half,-half),
                       pos+vec3(-half,half,half),vec3(0,1,0),mat_ptr);
        /*右面*/
        tg[4]=new triangle(pos+vec3(half,half,half),pos+vec3(half,half,-half),
                       pos+vec3(half,-half,half),vec3(1,0,0),mat_ptr);
        tg[5]=new triangle(pos+vec3(half,-half,-half),pos+vec3(half,half,-half),
                       pos+vec3(half,-half,half),vec3(1,0,0),mat_ptr);
        /*后面*/
        tg[6]=new triangle(pos+vec3(-half,half,-half),pos+vec3(half,half,-half),
                       pos+vec3(-half,-half,-half),vec3(0,0,-1),mat_ptr);
        tg[7]=new triangle(pos+vec3(half,-half,-half),pos+vec3(half,half,-half),
                       pos+vec3(-half,-half,-half),vec3(0,0,-1),mat_ptr);
        /*下面*/
        tg[8]=new triangle(pos+vec3(-half,-half,-half),pos+vec3(half,-half,-half),
                       pos+vec3(-half,-half,half),vec3(0,-1,0),mat_ptr);
        tg[9]=new triangle(pos+vec3(half,-half,half),pos+vec3(half,-half,-half),
                       pos+vec3(-half,-half,half),vec3(0,-1,0),mat_ptr);
        /*左面*/
        tg[10]=new triangle(pos+vec3(-half,half,half),pos+vec3(-half,half,-half),
                       pos+vec3(-half,-half,half),vec3(-1,0,0),mat_ptr);
        tg[11]=new triangle(pos+vec3(-half,-half,-half),pos+vec3(-half,half,-half),
                       pos+vec3(-half,-half,half),vec3(-1,0,0),mat_ptr);
    }
};

//康奈尔盒本质上就是法线全部翻转的正方体
class cornellBox
{
public:
    triangle* tg[120];
    int faceCnt=12;
    shared_ptr<material> mat_ptr[6];
public:
    cornellBox(){}
    cornellBox(point3 pos,double size,shared_ptr<material> mat[6]):
        mat_ptr{mat[0],mat[1],mat[2],mat[3],mat[4],mat[5]}
    {
        double half=size/2;
        /*前面*/
        tg[0]=new triangle(pos+vec3(-half,half,half),pos+vec3(half,half,half),
                       pos+vec3(-half,-half,half),vec3(0,0,-1),mat_ptr[0]);
        tg[1]=new triangle(pos+vec3(half,-half,half),pos+vec3(half,half,half),
                       pos+vec3(-half,-half,half),vec3(0,0,-1),mat_ptr[0]);
        /*上面*/
        tg[2]=new triangle(pos+vec3(-half,half,-half),pos+vec3(half,half,-half),
                       pos+vec3(-half,half,half),vec3(0,-1,0),mat_ptr[1]);
        tg[3]=new triangle(pos+vec3(half,half,half),pos+vec3(half,half,-half),
                       pos+vec3(-half,half,half),vec3(0,-1,0),mat_ptr[1]);
        /*右面*/
        tg[4]=new triangle(pos+vec3(half,half,half),pos+vec3(half,half,-half),
                       pos+vec3(half,-half,half),vec3(-1,0,0),mat_ptr[2]);
        tg[5]=new triangle(pos+vec3(half,-half,-half),pos+vec3(half,half,-half),
                       pos+vec3(half,-half,half),vec3(-1,0,0),mat_ptr[2]);
        /*后面*/
        tg[6]=new triangle(pos+vec3(-half,half,-half),pos+vec3(half,half,-half),
                       pos+vec3(-half,-half,-half),vec3(0,0,1),mat_ptr[3]);
        tg[7]=new triangle(pos+vec3(half,-half,-half),pos+vec3(half,half,-half),
                       pos+vec3(-half,-half,-half),vec3(0,0,1),mat_ptr[3]);
        /*下面*/
        tg[8]=new triangle(pos+vec3(-half,-half,-half),pos+vec3(half,-half,-half),
                       pos+vec3(-half,-half,half),vec3(0,1,0),mat_ptr[4]);
        tg[9]=new triangle(pos+vec3(half,-half,half),pos+vec3(half,-half,-half),
                       pos+vec3(-half,-half,half),vec3(0,1,0),mat_ptr[4]);
        /*左面*/
        tg[10]=new triangle(pos+vec3(-half,half,half),pos+vec3(-half,half,-half),
                       pos+vec3(-half,-half,half),vec3(1,0,0),mat_ptr[5]);
        tg[11]=new triangle(pos+vec3(-half,-half,-half),pos+vec3(-half,half,-half),
                       pos+vec3(-half,-half,half),vec3(1,0,0),mat_ptr[5]);
    }
};

class ground
{
public:
    triangle* tg[12];
    int faceCnt=2;
    shared_ptr<material> mat_ptr;
public:
    ground(){}
    ground(point3 pos,double half,shared_ptr<material> mat):mat_ptr(mat)
    {
        /*上面*/
        tg[0]=new triangle(pos+vec3(-half,0,-half),pos+vec3(half,0,-half),
                       pos+vec3(-half,0,half),vec3(0,1,0),mat_ptr,0,0,1,0,0,1);
        tg[1]=new triangle(pos+vec3(half,0,half),pos+vec3(half,0,-half),
                       pos+vec3(-half,0,half),vec3(0,1,0),mat_ptr,1,1,1,0,0,1);
    }
};

class wall
{
public:
    triangle* tg[12];
    int faceCnt=2;
    shared_ptr<material> mat_ptr;
public:
    wall(){}
    wall(point3 pos,double half,shared_ptr<material> mat):mat_ptr(mat)
    {
        /*上面*/
        tg[0]=new triangle(pos+vec3(-half,0,-half),pos+vec3(half,0,-half),
                       pos+vec3(-half,2*half,-half),vec3(0,0,1),mat_ptr,0,0,1,0,0,1);
        tg[1]=new triangle(pos+vec3(-half,2*half,-half),pos+vec3(half,0,-half),
                       pos+vec3(half,2*half,-half),vec3(0,0,1),mat_ptr,0,1,1,0,1,1);
    }
};

#endif // TRIANGLE_H
