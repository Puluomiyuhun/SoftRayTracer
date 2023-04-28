#include "vector4d.h"
#include "math.h"

void Vector4D::set(float tX,float tY,float tZ,float tW){
    x=tX;
    y=tY;
    z=tZ;
    w=tW;
}
void Vector4D::setX(float tX){
    x=tX;
}
void Vector4D::setY(float tY){
    y=tY;
}
void Vector4D::setZ(float tZ){
    z=tZ;
}
void Vector4D::setW(float tW){
    w=tW;
}
float Vector4D::magnitude(){
    return static_cast<float>
            (sqrt(static_cast<double>(x*x+y*y+z*z+w*w)));
}
Vector4D Vector4D::getNormalize(){
    float tmp=this->magnitude();
    Vector4D res(x/tmp,y/tmp,z/tmp,w/tmp);
    return res;
}
void Vector4D::normalize(){
    float tmp=this->magnitude();
    x/=tmp;
    y/=tmp;
    z/=tmp;
}

void Vector4D::operator=(const Vector3D &vec){
    x=vec.x;
    y=vec.y;
    z=vec.z;
    w=1;
}
Vector4D Vector4D::operator+(const Vector4D &vec)const {
    Vector4D res(x+vec.x,y+vec.y,z+vec.z,w+vec.w);
    return res;
}
Vector4D Vector4D::operator-(const Vector4D &vec)const {
    Vector4D res(x-vec.x,y+vec.y,z-vec.z,w-vec.w);
    return res;
}
Vector4D Vector4D::operator*(const float &t)const {
    Vector4D res(x*t,y*t,z*t,w*t);
    return res;
}
Vector4D Vector4D::operator/(const Vector4D &vec)const {
    if(vec.x==0.0f||vec.y==0.0f||vec.z==0.0f||vec.w==0.0f){
        Vector4D res(0.0f,0.0f,0.0f,0.0f);
        return res;
    }
    Vector4D res(x/vec.x,y/vec.y,z/vec.z,w/vec.w);
    return res;
}
void Vector4D::operator+=(const Vector4D &vec){
    x+=vec.x;
    y+=vec.y;
    z+=vec.z;
    w+=vec.w;
}
void Vector4D::operator-=(const Vector4D &vec){
    x-=vec.x;
    y-=vec.y;
    z-=vec.z;
    w-=vec.w;
}
void Vector4D::operator/=(const float &t){
    if(t==0.0f)return;
    x/=t;
    y/=t;
    z/=t;
    w/=t;
}
Vector4D Vector4D::operator+() const{
    return *this;
}
Vector4D Vector4D::operator-() const{
    Vector4D res(-x,-y,-z,-w);
    return res;
}
float Vector4D::dot(const Vector4D &vec) const{
    return x*vec.x+y*vec.y+z*vec.z+w*vec.w;
}
Vector4D Vector4D::lerp(const Vector4D &v2, float factor) const {
    Vector4D tmp(*this);
    return tmp*(1.0f-factor) + v2*factor;
}
