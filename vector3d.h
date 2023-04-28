#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D
{
public:
    float x,y,z;
public:
    Vector3D():x(0.0f),y(0.0f),z(0.0f){}
    Vector3D(float tX,float tY,float tZ):x(tX),y(tY),z(tZ){}
    Vector3D(const Vector3D &vec):x(vec.x),y(vec.y),z(vec.z){}
    ~Vector3D(){}

    void set(float tX,float tY,float tZ);
    void setX(float tX);
    void setY(float tY);
    void setZ(float tZ);
    float magnitude();
    Vector3D getNormalize();
    void normalize();

    Vector3D operator+(const Vector3D &vec);
    Vector3D operator-(const Vector3D &vec);
    Vector3D operator*(const float k);
    Vector3D operator/(const float k);
    void operator+=(const Vector3D &vec);
    void operator-=(const Vector3D &vec);
    void operator/=(const float k);
    Vector3D operator+() const;
    Vector3D operator-() const;

    float dot(const Vector3D &vec) const;
    Vector3D product(const Vector3D &vec) const;

    Vector3D lerp(const Vector3D &v2, float factor)const;
};

#endif // VECTOR3D_H
