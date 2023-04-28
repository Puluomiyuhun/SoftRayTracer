#include "postprocess.h"
#include "math.h"
#include "stdio.h"

int log(int k)
{
    int times=-1;
    while(k)
    {
        k/=2;
        times++;
    }
    return times;
}

postprocess::postprocess(unsigned char* c,int w,int h)
{
    width[0]=w;
    height[0]=h;
    for(int i=0;i<height[0];i++)
    {
        for(int j=0;j<width[0];j++)
        {
            mip[0][i][j][0]=c[(i*width[0]+j)*4];
            mip[0][i][j][1]=c[(i*width[0]+j)*4+1];
            mip[0][i][j][2]=c[(i*width[0]+j)*4+2];
        }
    }
    layer=log(height[0]);
    if(layer>10)layer=10;
}

void postprocess::BoxFilter()
{
    /*第一步：生成mipmap*/
    for(int l=1;l<=layer;l++)
    {
        for(int i=0;i<height[0];i++)
        {
            for(int j=0;j<width[0];j++)
            {
                int a=(int)(i/pow(2,l-1))*pow(2,l-1);
                int b=(int)(j/pow(2,l-1))*pow(2,l-1);
                mip[l][i][j][0]=mip[0][a][b][0];
                mip[l][i][j][1]=mip[0][a][b][1];
                mip[l][i][j][2]=mip[0][a][b][2];
            }
        }
    }

    /*第二步：分配层次权重*/
    double w[11];
    w[1]=1;
    for(int i=2;i<=layer;i++)
        w[i]=w[i-1]/1.4;
    //double w[11]={10,9,8,7,6,2,2,2,2,2,1};
    double normal=0;
    for(int i=1;i<=layer;i++)
        normal+=w[i];
    for(int i=1;i<=layer;i++)
        w[i]/=normal;

    /*第三步：滤波*/
    for(int i=0;i<height[0];i++)
    {
        for(int j=0;j<width[0];j++)
        {
            double r=0,g=0,b=0;
            for(int l=1;l<=layer;l++)
            {
                r+=(double)mip[l][i][j][0]*w[l];
                g+=(double)mip[l][i][j][1]*w[l];
                b+=(double)mip[l][i][j][2]*w[l];
            }
            mip[0][i][j][0]=(unsigned char)r;
            mip[0][i][j][1]=(unsigned char)g;
            mip[0][i][j][2]=(unsigned char)b;
        }
    }
}

void postprocess::mip2pixel(unsigned char* c)
{
    for(int i=0;i<height[0];i++)
    {
        for(int j=0;j<width[0];j++)
        {
            c[(i*width[0]+j)*4]=mip[0][i][j][0];
            c[(i*width[0]+j)*4+1]=mip[0][i][j][1];
            c[(i*width[0]+j)*4+2]=mip[0][i][j][2];
        }
    }
}
