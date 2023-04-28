#ifndef TEXTURE_H
#define TEXTURE_H
#include"vec3.h"
#include<QMainWindow>
#include<QString>

class texture
{
public:
    color col[256][256];
    texture(){}
};

class chessTexture : texture
{
public:
    color getCol(point3 p)
    {
        double tmp=sin(6 * p.x()) * sin(6 * p.y()) * sin(6 * p.z());
        if (tmp < 0)
            return color(0,0,0);
        else
            return color(1,1,1);
    }
};

class imageTexture :texture
{
public:
    int width,height;
    shared_ptr<QImage>pixelBuffer;
public:
    void loadImage(const QString &path)
    {
        pixelBuffer=make_shared<QImage>();
        pixelBuffer->load(path);
        width=pixelBuffer->width();
        height=pixelBuffer->height();
    }
    color getCol(double u,double v)
    {
        v = 1 - v;
        unsigned int x = 0, y = 0;
        if(u > 1.0f)
            u = u - static_cast<int>(u);
        else if(u < 0.0f)
            u = 1.0f - (static_cast<int>(u) - u);
        if(v > 1.0f)
            v = v - static_cast<int>(v);
        else if(v < 0.0f)
            v = 1.0f - (static_cast<int>(v) - v);
        double trueU = u * (width - 1);
        double trueV = v * (height - 1);
        x = static_cast<unsigned int>(trueU);
        y = static_cast<unsigned int>(trueV);
        color result(static_cast<float>(pixelBuffer->pixelColor(x,y).red()) * 1.0f/255,
                     static_cast<float>(pixelBuffer->pixelColor(x,y).green()) * 1.0f/255,
                     static_cast<float>(pixelBuffer->pixelColor(x,y).blue()) * 1.0f/255);
        return result;
    }
};

#endif // TEXTURE_H
