#ifndef SCENE_H
#define SCENE_H
#include"hittable_list.h"
#include"sphere.h"
#include"ray.h"
#include"camera.h"
#include"material.h"
#include"triangle.h"
#include"time.h"
#include"model.h"
#include<qapplication.h>

class scene
{
public:
    camera cam;
    hittable_list world;
public:
    scene(){};

    void random_scene(double a) {
        double aspect_ratio = a;
        point3 lookfrom(0, 1.5, 5);
        point3 lookat(0, 0, -1);
        /*point3 lookfrom(0, 1.5, -2);
        point3 lookat(3, 0, -1);*/
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.001;
        cam = camera(lookfrom, lookat, vup, 60, aspect_ratio, aperture, dist_to_focus);

        //auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));//地面
        auto mat = make_shared<lambertian>(color(0.4, 0.2, 0.1));
        auto ground_material = make_shared<lambertianChess>(make_shared<chessTexture>());
        world.add(new sphere(point3(0, -1000, 0), 1000, ground_material));
        srand(time(NULL));
        for (int a = -4; a < 4; a++) {
            for (int b = -6; b < 3; b++) {
                auto choose_mat = random_double(); //随机材质
                auto choose_shape = random_double(); //随机材质
                point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); //随机中心

                //if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                if(1){
                    shared_ptr<material> sphere_material;

                    if (choose_mat < 0.4) {
                        // diffuse 漫反射
                        auto albedo = color::random() * color::random();
                        sphere_material = make_shared<lambertian>(albedo);
                        if(choose_shape<0.7)
                            world.add(new sphere(center, 0.2, sphere_material));
                        else
                        {
                            auto c=cube(center,0.4,sphere_material);
                            for(int i=0;i<12;i++)
                                world.add(c.tg[i]);
                        }
                    }
                    else if (choose_mat < 0.6) {
                        // metal 金属
                        auto albedo = color::random(0.5, 1);
                        auto fuzz = random_double(0, 0.1);
                        sphere_material = make_shared<metal>(albedo, fuzz);
                        if(choose_shape<0.7)
                            world.add(new sphere(center, 0.2, sphere_material));
                        else
                        {
                            auto c=cube(center,0.4,sphere_material);
                            for(int i=0;i<12;i++)
                                world.add(c.tg[i]);
                        }
                    }
                    else {
                        // glass 玻璃
                        sphere_material = make_shared<dielectric>(1.5);
                        if(choose_shape<0.7)
                            world.add(new sphere(center, 0.2, sphere_material));
                        else
                        {
                            auto c=cube(center,0.4,sphere_material);
                            for(int i=0;i<12;i++)
                                world.add(c.tg[i]);
                        }
                    }
                }
            }
        }

        auto material1 = make_shared<dielectric>(1.5);
        world.add(new sphere(point3(4, 0.7, 0), 0.5, material1));

        auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
        world.add(new sphere(point3(-4, 0.7, 0), 0.7, material2));

        auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(new sphere(point3(0, 0.5, 0), 0.7, material3));

        return;
    }

    void cornell_scene(double a)
    {
        double aspect_ratio = a;
        point3 lookfrom(0, 10, -0.1);
        point3 lookat(0, 10, -5);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.001;
        cam = camera(lookfrom, lookat, vup, 85, aspect_ratio, aperture, dist_to_focus);
        /*康奈尔盒*/
        point3 center(0,10,-10);
        shared_ptr<material> mat[6]=  //前上右后下左
        {
            /*make_shared<lambertian>(color(0,0,1)),
            make_shared<lambertian>(color(1,1,1)),
            make_shared<lambertian>(color(1,0,0)),
            make_shared<lambertian>(color(1,1,1)),
            make_shared<lambertian>(color(1,1,1)),
            make_shared<lambertian>(color(0,1,0))*/

            /*make_shared<metal>(color(1,1,1),0.05),
            make_shared<lambertian>(color(0.8,0.8,0.8)),
            make_shared<metal>(color(1,1,1),0.05),
            make_shared<lambertian>(color(0.6,0.6,0.6)),
            make_shared<metal>(color(1,1,1),0.2),
            make_shared<metal>(color(1,1,1),0.05),*/

            make_shared<lambertian>(color(1,1,1)),
            make_shared<lambertian>(color(1,1,1)),
            make_shared<lambertian>(color(1,0,0)),
            make_shared<lambertian>(color(0,0,1)),
            make_shared<lambertian>(color(1,1,1)),
            make_shared<lambertian>(color(0,1,0))
        };
        auto c=cornellBox(center,20,mat);
        for(int i=0;i<12;i++)
            world.add(c.tg[i]);

        /*光源*/
        point3 pos(0,19.9,-15);
        float half = 3;
        shared_ptr<material> matLight =
                make_shared<light>(color(1,1,1),3);
        auto l1=new triangle(pos+vec3(-half,0,-half),pos+vec3(half,0,-half),
                       pos+vec3(-half,0,half),vec3(0,-1,0),matLight);
        auto l2=new triangle(pos+vec3(half,0,half),pos+vec3(half,0,-half),
                       pos+vec3(-half,0,half),vec3(0,-1,0),matLight);
        world.add(l1);
        world.add(l2);

        /*实体*/
        //auto material = make_shared<lambertian>(color(1,0,0));
        auto material1 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(new sphere(point3(5, 5, -14), 4, material1));
        auto materialc = make_shared<lambertian>(color(0.7,0.7,0));
        auto cubes=cube(point3(-5, 2, -14),4,materialc);
        for(int i=0;i<12;i++)
            world.add(cubes.tg[i]);
        return;
    }

    void test(double a)
    {
        double aspect_ratio = a;
        point3 lookfrom(0, 3, 6.5);
        point3 lookat(0, 0, -1.5);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.001;
        cam = camera(lookfrom, lookat, vup, 60, aspect_ratio, aperture, dist_to_focus);
        auto mat = make_shared<lambertian>(color(0.4, 0.2, 0.1));
        //auto ground_material = make_shared<lambertianChess>(make_shared<chessTexture>());
        auto ground_material = make_shared<metal>(color(0.8, 0.8, 1), 0.04);
        world.add(new sphere(point3(0, -1000, 0), 1000, ground_material));

        auto material1 = make_shared<lambertianImage>("D:/texture/floor.jpg",2,2);
        world.add(new sphere(point3(0, 1, 2), 1, material1));
        auto material2 = make_shared<lambertianImage>("D:/texture/ground.jpg",2,2);
        world.add(new sphere(point3(-2, 1, 0), 1, material2));
        auto material3 = make_shared<lambertianImage>("D:/texture/wall.jpg",2,2);
        world.add(new sphere(point3(2, 1, 0), 1, material3));
        auto material4 = make_shared<lambertianImage>("D:/texture/ci.jpg",2,2);
        world.add(new sphere(point3(4, 1, -2), 1, material4));
        auto material5 = make_shared<lambertianImage>("D:/texture/mud.jpg",1,1);
        world.add(new sphere(point3(1.1, 1, -2), 1, material5));
        auto material6 = make_shared<lambertianImage>("D:/texture/ice.jpg",1,1);
        world.add(new sphere(point3(-1.1, 1, -2), 1, material6));
        auto material7 = make_shared<lambertianImage>("D:/texture/grass.jpg",1,1);
        world.add(new sphere(point3(-4, 1, -2), 1, material7));
        auto material8 = make_shared<lambertianImage>("D:/texture/al.jpg",1,1);
        world.add(new sphere(point3(6, 1, -4), 1, material8));
        auto material9 = make_shared<lambertianImage>("D:/texture/zhuan.jpg",1,1);
        world.add(new sphere(point3(3, 1, -4), 1, material9));
        auto material10 = make_shared<lambertianImage>("D:/texture/road.jpg",1,1);
        world.add(new sphere(point3(0, 1, -4), 1, material10));
        auto material11 = make_shared<lambertianImage>("D:/texture/sofa.jpg",1,1);
        world.add(new sphere(point3(-3, 1, -4), 1, material11));
        auto material12 = make_shared<lambertianImage>("D:/texture/cu.jpg",1,1);
        world.add(new sphere(point3(-6, 1, -4), 1, material12));
        return;
    }

    void model_scene(double a)
    {
        double aspect_ratio = a;
        //point3 lookfrom(8, 5, 8);
        //point3 lookfrom(3, 3, 2);
        //point3 lookfrom(-8, 5, 18);
        point3 lookfrom(8, 5, 24);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.000;
        cam = camera(lookfrom, lookat, vup, 60, aspect_ratio, aperture, dist_to_focus);

        /*小球*/
        for(int i = 0;i<10;i++){
            auto matLight = make_shared<light>(color(random_double(),random_double(),random_double()),3);
            auto light=new sphere(point3(-8 + random_double()*15,0.4f, 2 + random_double()*15), 0.4f,matLight);
            world.add(light);
        }
        for(int i = 0;i<10;i++){
            auto wall3_material = make_shared<metal>(color(1, 1, 1), 0.0);
            auto light=new sphere(point3(-8 + random_double()*15,0.6f, 2 + random_double()*15), 0.6f,wall3_material);
            world.add(light);
        }
        for(int i = 0;i<10;i++){
            auto wall3_material = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
            auto light=new sphere(point3(-8 + random_double()*15,0.5f, 2 + random_double()*15), 0.5f,wall3_material);
            world.add(light);
        }

        /*世界*/
        auto ground_material = make_shared<lambertianImage>("D:/texture/ground.jpg",20,20);
        ground gro=ground(vec3(0,0,0),100,ground_material);
        world.add(gro.tg[0]);
        world.add(gro.tg[1]);
        auto wall_material = make_shared<lambertianImage>("D:/texture/wall.jpg",5,5);
        wall wal=wall(vec3(0,-1,-2),25,wall_material);
        world.add(wal.tg[0]);
        world.add(wal.tg[1]);
        auto wall3_material = make_shared<metal>(color(1, 1, 1), 0.0);
        world.add(new sphere(point3(10, 4, -2), 4, wall3_material));
        world.add(new sphere(point3(-3, 4, -6), 4, wall3_material));

        /*模型*/
        model d=model(QApplication::applicationDirPath()+"/lisa1.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        d=model(QApplication::applicationDirPath()+"/lisa2.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        d=model(QApplication::applicationDirPath()+"/lisa3.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        d=model(QApplication::applicationDirPath()+"/chair.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        /*auto matmirror = make_shared<metal>(color(0.5,0.5,0.5), 0.01);
        d=model(QApplication::applicationDirPath()+"/mirror.obj",matmirror);
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }

        auto sphere_material = make_shared<dielectric>(1.15);
        model d=model(QApplication::applicationDirPath()+"/zhanju.obj",sphere_material);
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        d=model(QApplication::applicationDirPath()+"/jian.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }*/
        d=model(QApplication::applicationDirPath()+"/zuo.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        d=model(QApplication::applicationDirPath()+"/you.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        d=model(QApplication::applicationDirPath()+"/yangtou.obj");
        for(int i=0;i<d.faceCnt;i++){
            world.add(d.tg[i]);
        }
        return;
    }
};

#endif // SCENE_H
