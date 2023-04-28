#include "model.h"
#include "QTextStream"
#include <QDebug>
#include "material.h"

model::model(QString path){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        printf("faild\n");
        return;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    int pocnt=0,tecnt=0,ncnt=0;
    QString mtlpath="";
    shared_ptr<material> mat[15];
    int matIndex=0;
    while(!in.atEnd())
    {
        QString onecount = in.readLine();
        std::string str=onecount.toStdString();
        /*读取材质文件名称*/
        if(str[0]=='m')
        {
            QStringList list = path.split("/");
            for(int i=0;i<list.size()-1;i++)
                mtlpath=mtlpath.append(list[i]+"/");
            for(int i=7;i<onecount.size();i++)
                mtlpath=mtlpath.append(str[i]);
        }
        /*读取顶点*/
        if(str[0]=='v'&& str[1]==' ')
        {
            pocnt++;
            int index=0;float num[4]={0,0,0,0};int flag[4]={1,1,1,1};
            bool po=false;int bit=0;
            for(unsigned int i=1;i<str.length();i++)
            {
                if(str[i]==' ')
                {
                    index++;
                    po=false;
                    bit=0;
                }
                else if(str[i]>='0'&&str[i]<='9')
                {
                    if(po==false){
                        num[index]=num[index]*10+str[i]-'0';
                    }
                    else{
                        num[index]+=(str[i]-'0')*powf(10,-bit);
                        bit++;
                    }
                }
                else if(str[i]=='.')
                {
                    po=true;
                    bit=1;
                }
                else if(str[i]=='-')flag[index]=-1;
            }
            point[pocnt]=vec3(num[1]*flag[1],num[2]*flag[2],num[3]*flag[3]);
        }
        /*读取纹理坐标*/
        if(str[0]=='v'&& str[1]=='t')
        {
            tecnt++;
            int index=0;float num[3]={0,0,0};
            bool po=false;int bit=0;
            for(unsigned int i=2;i<str.length();i++)
            {
                if(str[i]==' ')
                {
                    index++;
                    po=false;
                    bit=0;
                }
                else if(str[i]>='0'&&str[i]<='9')
                {
                    if(po==false){
                        num[index]=num[index]*10+str[i]-'0';
                    }
                    else{
                        num[index]+=(str[i]-'0')*powf(10,-bit);
                        bit++;
                    }
                }
                else if(str[i]=='.')
                {
                    po=true;
                    bit=1;
                }
            }
            tex[tecnt]=vec3(num[1],num[2],0);
        }
        /*读取法向量*/
        if(str[0]=='v'&& str[1]=='n')
        {
            ncnt++;
            int index=0;float num[4]={0,0,0,0};int flag[4]={1,1,1,1};
            bool po=false;int bit=0;
            for(unsigned int i=2;i<str.length();i++)
            {
                if(str[i]==' ')
                {
                    index++;
                    po=false;
                    bit=0;
                }
                else if(str[i]>='0'&&str[i]<='9')
                {
                    if(po==false){
                        num[index]=num[index]*10+str[i]-'0';
                    }
                    else{
                        num[index]+=(str[i]-'0')*powf(10,-bit);
                        bit++;
                    }
                }
                else if(str[i]=='.')
                {
                    po=true;
                    bit=1;
                }
                else if(str[i]=='-')flag[index]=-1;
            }
            nor[ncnt]=vec3(num[1]*flag[1],num[2]*flag[2],num[3]*flag[3]);
        }
        /*读取材质*/
        else if(str[0]=='u')
        {
            matIndex++;
            char mtlname[100];
            for(int i=7;i<onecount.size();i++)
                mtlname[i-7]=str[i];
            mtlname[onecount.size()-7]='\0';
            QFile mtlfile(mtlpath);
            if(!mtlfile.open(QIODevice::ReadOnly)){
                printf("faild\n");
                return;
            }
            QTextStream in2(&mtlfile);
            bool found=false;
            while(!in2.atEnd())
            {
                QString onecount2 = in2.readLine();
                std::string str2=onecount2.toStdString();
                if(str2[0]=='n')
                {
                    char mtlname2[100];
                    for(int i=7;i<onecount2.size();i++)
                        mtlname2[i-7]=str2[i];
                    mtlname2[onecount2.size()-7]='\0';
                    if(strcmp(mtlname,mtlname2)==0)
                        found=true;
                }
                if(str2[0]=='m'&&found==true)
                {
                    QString texpath="";
                    for(int i=7;i<onecount2.size();i++)
                    {
                        if(str2[i]=='\\')
                        {
                            texpath.append("/");
                            i++;
                        }
                        else
                            texpath.append(str2[i]);
                    }
                    mat[matIndex]=make_shared<lambertianImage>(texpath,1,1);
                    break;
                }
            }
            mtlfile.close();
        }
        /*读取面*/
        else if(str[0]=='f')
        {
            int pointIndex=0,infIndex,num;
            for(unsigned int i=1;i<=str.length();i++)
            {
                if(i==str.length()){
                    inf[pointIndex][infIndex]=num;
                    break;
                }
                if(str[i]==' ')
                {
                    if(pointIndex>0)inf[pointIndex][infIndex]=num;
                    pointIndex++;
                    infIndex=1;
                    num=0;
                }
                else if(str[i]>='0'&&str[i]<='9'){
                    num=num*10+str[i]-'0';
                }
                else if(str[i]=='/'){
                    inf[pointIndex][infIndex]=num;
                    infIndex++;
                    num=0;
                }
            }
            for(int i=3;i<=pointIndex;i++)
            {
                /*1、i-1、i三个点*/
                vec3 normal=(nor[inf[1][3]]+nor[inf[i-1][3]]+nor[inf[i][3]])/3;
                tg[faceCnt]=new triangle
                        (point[inf[1][1]],point[inf[i-1][1]],point[inf[i][1]],
                        normal,mat[matIndex],
                        tex[inf[1][2]][0],tex[inf[1][2]][1],
                        tex[inf[i-1][2]][0],tex[inf[i-1][2]][1],
                        tex[inf[i][2]][0],tex[inf[i][2]][1]
                        );
                faceCnt++;
            }
        }
    }
    file.close();
}

model::model(QString path, shared_ptr<material> mat){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        printf("faild\n");
        return;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    int pocnt=0,tecnt=0,ncnt=0;
    while(!in.atEnd())
    {
        QString onecount = in.readLine();
        std::string str=onecount.toStdString();
        /*读取顶点*/
        if(str[0]=='v'&& str[1]==' ')
        {
            pocnt++;
            int index=0;float num[4]={0,0,0,0};int flag[4]={1,1,1,1};
            bool po=false;int bit=0;
            for(unsigned int i=1;i<str.length();i++)
            {
                if(str[i]==' ')
                {
                    index++;
                    po=false;
                    bit=0;
                }
                else if(str[i]>='0'&&str[i]<='9')
                {
                    if(po==false){
                        num[index]=num[index]*10+str[i]-'0';
                    }
                    else{
                        num[index]+=(str[i]-'0')*powf(10,-bit);
                        bit++;
                    }
                }
                else if(str[i]=='.')
                {
                    po=true;
                    bit=1;
                }
                else if(str[i]=='-')flag[index]=-1;
            }
            point[pocnt]=vec3(num[1]*flag[1],num[2]*flag[2],num[3]*flag[3]);
        }
        /*读取纹理坐标*/
        if(str[0]=='v'&& str[1]=='t')
        {
            tecnt++;
            int index=0;float num[3]={0,0,0};
            bool po=false;int bit=0;
            for(unsigned int i=2;i<str.length();i++)
            {
                if(str[i]==' ')
                {
                    index++;
                    po=false;
                    bit=0;
                }
                else if(str[i]>='0'&&str[i]<='9')
                {
                    if(po==false){
                        num[index]=num[index]*10+str[i]-'0';
                    }
                    else{
                        num[index]+=(str[i]-'0')*powf(10,-bit);
                        bit++;
                    }
                }
                else if(str[i]=='.')
                {
                    po=true;
                    bit=1;
                }
            }
            tex[tecnt]=vec3(num[1],num[2],0);
        }
        /*读取法向量*/
        if(str[0]=='v'&& str[1]=='n')
        {
            ncnt++;
            int index=0;float num[4]={0,0,0,0};int flag[4]={1,1,1,1};
            bool po=false;int bit=0;
            for(unsigned int i=2;i<str.length();i++)
            {
                if(str[i]==' ')
                {
                    index++;
                    po=false;
                    bit=0;
                }
                else if(str[i]>='0'&&str[i]<='9')
                {
                    if(po==false){
                        num[index]=num[index]*10+str[i]-'0';
                    }
                    else{
                        num[index]+=(str[i]-'0')*powf(10,-bit);
                        bit++;
                    }
                }
                else if(str[i]=='.')
                {
                    po=true;
                    bit=1;
                }
                else if(str[i]=='-')flag[index]=-1;
            }
            nor[ncnt]=vec3(num[1]*flag[1],num[2]*flag[2],num[3]*flag[3]);
        }
        /*读取面*/
        else if(str[0]=='f')
        {
            int pointIndex=0,infIndex,num;
            for(unsigned int i=1;i<=str.length();i++)
            {
                if(i==str.length()){
                    inf[pointIndex][infIndex]=num;
                    break;
                }
                if(str[i]==' ')
                {
                    if(pointIndex>0)inf[pointIndex][infIndex]=num;
                    pointIndex++;
                    infIndex=1;
                    num=0;
                }
                else if(str[i]>='0'&&str[i]<='9'){
                    num=num*10+str[i]-'0';
                }
                else if(str[i]=='/'){
                    inf[pointIndex][infIndex]=num;
                    infIndex++;
                    num=0;
                }
            }
            for(int i=3;i<=pointIndex;i++)
            {
                /*1、i-1、i三个点*/
                vec3 normal=(nor[inf[1][3]]+nor[inf[i-1][3]]+nor[inf[i][3]])/3;
                tg[faceCnt]=new triangle
                        (point[inf[1][1]],point[inf[i-1][1]],point[inf[i][1]],
                        normal,mat,
                        tex[inf[1][2]][0],tex[inf[1][2]][1],
                        tex[inf[i-1][2]][0],tex[inf[i-1][2]][1],
                        tex[inf[i][2]][0],tex[inf[i][2]][1]
                        );
                faceCnt++;
            }
        }
    }
    file.close();
}
