#include "vector3d.h"
#include "math.h"

void Vector3D::set(float tX,float tY,float tZ){
    x=tX;
    y=tY;
    z=tZ;
}
void Vector3D::setX(float tX){
    x=tX;
}
void Vector3D::setY(float tY){
    y=tY;
}
void Vector3D::setZ(float tZ){
    z=tZ;
}
float Vector3D::magnitude(){
    return static_cast<float>
            (sqrt(static_cast<double>(x*x+y*y+z*z)));
}
Vector3D Vector3D::getNormalize(){
    float tmp=this->magnitude();
    Vector3D res(x/tmp,y/tmp,z/tmp);
    return res;
}
void Vector3D::normalize(){
    float tmp=this->magnitude();
    x/=tmp;
    y/=tmp;
    z/=tmp;
}

Vector3D Vector3D::operator+(const Vector3D &vec){
    Vector3D res(x+vec.x,y+vec.y,z+vec.z);
    return res;
}
Vector3D Vector3D::operator-(const Vector3D &vec){
    Vector3D res(x-vec.x,y-vec.y,z-vec.z);
    return res;
}
Vector3D Vector3D::operator*(const float k){
    Vector3D res(x*k,y*k,z*k);
    return res;
}
Vector3D Vector3D::operator/(const float k){
    if(k==0.0f){
        Vector3D res(0.0f,0.0f,0.0f);
        return res;
    }
    Vector3D res(x/k,y/k,z/k);
    return res;
}
void Vector3D::operator+=(const Vector3D &vec){
    x+=vec.x;
    y+=vec.y;
    z+=vec.z;
}
void Vector3D::operator-=(const Vector3D &vec){
    x-=vec.x;
    y-=vec.y;
    z-=vec.z;
}
void Vector3D::operator/=(const float k){
    if(k!=0.0f)x/=k;
    else x=0.0f;
    if(k!=0.0f)y/=k;
    else y=0.0f;
    if(k!=0.0f)z/=k;
    else z=0.0f;
}
Vector3D Vector3D::operator+() const{
    return *this;
}
Vector3D Vector3D::operator-() const{
    Vector3D res(-x,-y,-z);
    return res;
}
float Vector3D::dot(const Vector3D &vec) const{
    return x*vec.x+y*vec.y+z*vec.z;
}
Vector3D Vector3D::product(const Vector3D &vec) const{
    Vector3D res(y*vec.z-z*vec.y,z*vec.x-x*vec.z,x*vec.y-y*vec.x);
    return res;
}
Vector3D Vector3D::lerp(const Vector3D &v2, float factor)const{
    Vector3D tmp(*this),v(v2);
    return tmp*(1.0f-factor) + v*factor;
}
