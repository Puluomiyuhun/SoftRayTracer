#ifndef MODEL_H
#define MODEL_H
#include <QFileDialog>
#include"hittable.h"
#include"vec3.h"
#include "triangle.h"

class model
{
public:
    vec3 *point=new vec3[32000];
    vec3 *tex=new vec3[32000];
    vec3 *nor=new vec3[32000];
    int inf[13000][3];
    triangle* tg[32000];
    int faceCnt=0;
    shared_ptr<material> mat_ptr;
public:
    model(QString path);
    model(QString path,shared_ptr<material> mat);
};

#endif // MODEL_H
