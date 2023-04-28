#ifndef VECTOR4D_H
#define VECTOR4D_H
#include "vector3d.h"

class Vector4D
{
public:
    float x,y,z,w;
public:
    Vector4D():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}
    Vector4D(float tX,float tY,float tZ,float tW):x(tX),y(tY),z(tZ),w(tW){}
    Vector4D(const Vector4D &vec):x(vec.x),y(vec.y),z(vec.z),w(vec.w){}
    ~Vector4D(){}

    void set(float tX,float tY,float tZ,float tW);
    void setX(float tX);
    void setY(float tY);
    void setZ(float tZ);
    void setW(float tW);
    float magnitude();
    Vector4D getNormalize();
    void normalize();

    void operator=(const Vector3D &vec);
    Vector4D operator+(const Vector4D &vec)const ;
    Vector4D operator-(const Vector4D &vec)const ;
    Vector4D operator*(const float &t)const ;
    Vector4D operator/(const Vector4D &vec)const ;
    void operator+=(const Vector4D &vec);
    void operator-=(const Vector4D &vec);
    void operator/=(const float &t);
    Vector4D operator+() const;
    Vector4D operator-() const;

    float dot(const Vector4D &vec) const;
    Vector4D lerp(const Vector4D &v2, float factor)const;
};

#endif // VECTOR4D_H
