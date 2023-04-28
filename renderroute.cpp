#include "renderroute.h"
#include<iostream>
#include"scene.h"
#include"postprocess.h"

using std::cin;
using std::cout;

const int max_depth = 10;
const int samples_per_pixel = 2048;

void RenderRoute::write_color(int h, int w, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    //根据样本数对颜色取平均值
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);
    col[(h*image_width+w)*4]=static_cast<int>(256 * clamp(r, 0.0, 0.999));
    col[(h*image_width+w)*4+1]=static_cast<int>(256 * clamp(g, 0.0, 0.999));
    col[(h*image_width+w)*4+2]=static_cast<int>(256 * clamp(b, 0.0, 0.999));
    col[(h*image_width+w)*4+3]=255;
}
/*void RenderRoute::write_color(int h, int w, color pixel_color, int spp) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    //根据样本数对颜色取平均值
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);
    col[(h*image_width+w)*4]=(col[(h*image_width+w)*4]*spp+static_cast<int>(256 * clamp(r, 0.0, 0.999)))/(spp+1);
    col[(h*image_width+w)*4+1]=(col[(h*image_width+w)*4+1]*spp+static_cast<int>(256 * clamp(g, 0.0, 0.999)))/(spp+1);
    col[(h*image_width+w)*4+2]=(col[(h*image_width+w)*4+2]*spp+static_cast<int>(256 * clamp(b, 0.0, 0.999)))/(spp+1);
    col[(h*image_width+w)*4+3]=255;
}*/

color RenderRoute::ray_color(const ray& r, const hittable& world, int depth) {
    if(depth <= 0)
        return color(0, 0, 0);
    hit_record rec;
    vec3 result = color(0, 0, 0);
    if (world.hit(r, 0.01, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            if(rec.mat_ptr->isLight == true){
                return vec3(3,3,3);
            }
            float random_goal_y = random_double(17,23);
            float random_goal_z = random_double(-18,-12);
            vec3 to_light = (vec3(0,random_goal_y,random_goal_z)-rec.p).normalize();
            double distance = (vec3(0,random_goal_y,random_goal_z)-rec.p).length();
            double pdf = 1.0f / (3 * 3);
            if(rec.mat_ptr->isMetal == false)
                result += vec3(10,10,10) * attenuation * dot(rec.normal,to_light) * fabs(dot(-to_light,vec3(0,-1,0))) / (distance * distance) / pdf;
            return result + attenuation * ray_color(scattered, world, depth - 1) * dot(scattered.direction().normalize(), rec.normal);
        }
        return vec3(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    double t = 1 - 3 * (unit_direction.y());
    if(depth>=max_depth)
        return t * color(0.4, 0.1, 0.1) + (1 - t) * color(0.1f, 0.1f, 0.3f);
    else
        return t * color(0.3, 0.2, 0.2) + (1 - t) * color(0.3f, 0.3f, 0.3f);
    /*if(depth==max_depth)
        return (1.0 - t) * color(0.8, 0.8, 1.0) + t * color(0.5, 0.7, 1.0);
    else
        return color(0.7, 0.7, 0.7);*/
}

void RenderRoute::loop() {
    emit frameOut(col);
    auto world = sc->world;
    bvh_node head = bvh_node(world.objects,world.count);
    auto cam = sc->cam;
    camera_pos=cam.getOrigin();
/*固定采样数*/
#pragma omp parallel for
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, head, max_depth);
            }
            write_color(image_height-j-1,i,pixel_color, samples_per_pixel);//取平均值
            emit frameOut(col);
        }
    }
/*递增采样数*/
    /*
    int spp_now=0;
    while(1)
    {
    #pragma omp parallel for
        for (int j = image_height - 1; j >= 0; --j) {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, head, max_depth);
                write_color(image_height-j-1,i,pixel_color, spp_now);//取平均值
                emit frameOut(col);
            }
        }
        spp_now++;
        printf("%d\n",spp_now);fflush(stdout);
    }
    */
    /*后处理阶段*/
    //postprocess *pp=new postprocess(col,image_width,image_height);
    //pp->BoxFilter();
    //pp->mip2pixel(col);
    //emit frameOut(col);
}
