#ifndef MATRIX_H
#define MATRIX_H
#include "vector3d.h"
#include "vector4d.h"

class Matrix
{
public:
    float ele[4][4];
public:
    Matrix():ele{0.0f}{}

    Matrix(float a00,float a01,float a02,float a03,
           float a10,float a11,float a12,float a13,
           float a20,float a21,float a22,float a23,
           float a30,float a31,float a32,float a33)
        :ele{a00,a01,a02,a03,a10,a11,a12,a13,a20,a21,a22,a23,a30,a31,a32,a33}{}

    Matrix(const Matrix &mat)
        :ele{mat.ele[0][0],mat.ele[0][1],mat.ele[0][2],mat.ele[0][3],
             mat.ele[1][0],mat.ele[1][1],mat.ele[1][2],mat.ele[1][3],
             mat.ele[2][0],mat.ele[2][1],mat.ele[2][2],mat.ele[2][3],
             mat.ele[3][0],mat.ele[3][1],mat.ele[3][2],mat.ele[3][3]}{}

    ~Matrix(){}

    void set(float e,int x,int y);

    Matrix operator+(const Matrix &mat);
    Matrix operator-(const Matrix &mat);
    Matrix operator*(const float k);
    Matrix operator*(const Matrix &mat);
    Vector4D operator*(const Vector4D vec);
    Matrix operator/(const float k);
    void operator+=(const Matrix &mat);
    void operator-=(const Matrix &mat);
    void operator*=(const float k);
    void operator*=(const Matrix &mat);
    void operator/=(const float k);
    Matrix operator+() const;
    Matrix operator-() const;

    void normalize();
    void inverse();
    void transposition();
    void translation(const Vector3D & trans);
    void scale(const Vector3D & sca);
    void rotationX(const double angle);
    void rotationY(const double angle);
    void rotationZ(const double angle);
    void perspective(float fovy, float aspect, float near, float far);
    void perspective(float left, float right, float bottom, float top, float near, float far);
    void ortho(float left, float right, float bottom, float top, float near, float far);

    void lookat(Vector3D camPos,Vector3D tarPos,Vector3D up);
    void viewPort(int left,int top,int width,int height);
};

#endif // MATRIX_H
