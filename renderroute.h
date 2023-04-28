#ifndef RENDERROUTE_H
#define RENDERROUTE_H
#include"QObject"
#include"vec3.h"
#include"scene.h"
#include"bvh_node.h"

class RenderRoute:public QObject
{
    Q_OBJECT
public:
    explicit RenderRoute(int w, int h, QObject *parent, scene *s)
        : QObject(parent), image_width(w), image_height(h), sc(s){};
    ~RenderRoute(){}
signals:
    void frameOut(unsigned char *image);

public slots:
    void loop();

private:
    void write_color(int h, int w, color pixel_color, int samples_per_pixel);
    color ray_color(const ray& r, const hittable& world, int depth);

private:
    bool stopped;
    int image_width,image_height;
    //unsigned char col[8294405];
    scene *sc;
public:
    unsigned char col[8294405];
};

#endif
