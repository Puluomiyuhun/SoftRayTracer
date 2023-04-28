#ifndef LIGHT_H
#define LIGHT_H
#include"vec3.h"

class light
{
public:
    color lightColor;
    float itensity;
public:
    light();
    light(color i,float a):lightColor(i),itensity(a){};
};

#endif // LIGHT_H
