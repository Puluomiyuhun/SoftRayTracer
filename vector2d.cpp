#include "vector2d.h"
#include "math.h"

void Vector2D::set(float tX,float tY){
    x=tX;
    y=tY;
}
void Vector2D::setX(float tX){
    x=tX;
}
void Vector2D::setY(float tY){
    y=tY;
}
float Vector2D::magnitude(){
    return static_cast<float>
            (sqrt(static_cast<double>(x*x+y*y)));
}
Vector2D Vector2D::getNormalize(){
    float tmp=this->magnitude();
    Vector2D res(x/tmp,y/tmp);
    return res;
}
void Vector2D::normalize(){
    float tmp=this->magnitude();
    x/=tmp;
    y/=tmp;
}

Vector2D Vector2D::operator+(const Vector2D &vec){
    Vector2D res(x+vec.x,y+vec.y);
    return res;
}
Vector2D Vector2D::operator-(const Vector2D &vec){
    Vector2D res(x-vec.x,y-vec.y);
    return res;
}
Vector2D Vector2D::operator*(const float k){
    Vector2D res(x*k,y*k);
    return res;
}
Vector2D Vector2D::operator/(const float k){
    if(k==0.0f){
        Vector2D res(0.0f,0.0f);
        return res;
    }
    Vector2D res(x/k,y/k);
    return res;
}
void Vector2D::operator+=(const Vector2D &vec){
    x+=vec.x;
    y+=vec.y;
}
void Vector2D::operator-=(const Vector2D &vec){
    x-=vec.x;
    y-=vec.y;
}
void Vector2D::operator*=(const float k){
    x*=k;
    y*=k;
}
void Vector2D::operator/=(const float k){
    if(k!=0.0f)x/=k;
    else x=0.0f;
    if(k!=0.0f)y/=k;
    else y=0.0f;
}
Vector2D Vector2D::operator+() const{
    return *this;
}
Vector2D Vector2D::operator-() const{
    Vector2D res(-x,-y);
    return res;
}
Vector2D Vector2D::lerp(const Vector2D &v2, float factor) const {
    Vector2D tmp(*this),v(v2);
    return tmp*(1.0f-factor) + v*factor;
}
