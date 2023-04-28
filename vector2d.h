#ifndef VECTOR2D_H
#define VECTOR2D_H


class Vector2D
{
public:
    float x,y;
public:
    Vector2D():x(0.0f),y(0.0f){}
    Vector2D(float tX,float tY):x(tX),y(tY){}
    Vector2D(const Vector2D &vec):x(vec.x),y(vec.y){}
    ~Vector2D(){}

    void set(float tX,float tY);
    void setX(float tX);
    void setY(float tY);
    float magnitude();
    Vector2D getNormalize();
    void normalize();

    Vector2D operator+(const Vector2D &vec);
    Vector2D operator-(const Vector2D &vec);
    Vector2D operator*(const float k);
    Vector2D operator/(const float k);
    void operator+=(const Vector2D &vec);
    void operator-=(const Vector2D &vec);
    void operator*=(const float k);
    void operator/=(const float k);
    Vector2D operator+() const;
    Vector2D operator-() const;

    Vector2D lerp(const Vector2D &v2, float factor)const;
};

#endif // VECTOR2D_H
