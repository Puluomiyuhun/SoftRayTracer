#ifndef POSTPROCESS_H
#define POSTPROCESS_H

class postprocess
{
public:
    unsigned char mip[11][1085][1925][3];
    int width[11];
    int height[11];
    int layer;
public:
    postprocess(unsigned char* c,int w,int h);
    void BoxFilter();
    void mip2pixel(unsigned char* c);
};

#endif // POSTPROCESS_H
