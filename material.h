#ifndef  MATERIAL_H
#define  MATERIAL_H

#include"rtweekend.h"
#include"hittable.h"
#include"texture.h"
#include"disney.h"

class material {
public:
    bool isLight = false;
    bool isMetal = false;
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
    // 将向量 v 投影到 N 的法向半球
    vec3 toNormalHemisphere(vec3 v, vec3 N)const {
        vec3 helper = vec3(1, 0, 0);
        if(fabs(N.x())>0.999) helper = vec3(0, 1, 0);
        vec3 tangent = cross(N, helper).normalize();
        vec3 bitangent = cross(N, tangent).normalize();
        return v.x() * tangent + v.z() * bitangent + v.y() * N;
    }
    vec3 SampleCosineHemisphere(float xi_1, float xi_2, vec3 N)const {
        float r = sqrt(xi_1);
        float theta = xi_2 * 2.0 * pi;
        float x = r * cos(theta);
        float z = r * sin(theta);
        float y = sqrt(1.0 - xi_1);
        vec3 L = toNormalHemisphere(vec3(x, y, z), N);
        return L;
    }
};

class lambertian :public material {
public:
    color albedo;
public:
    lambertian(const color& a) :albedo(a) { }
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            /*vec3 scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo * dot(scattered.direction(), rec.normal);*/
            /*float U1=random_double(0,1),U2=random_double(0,1);
            vec3 L=SampleCosineHemisphere(U1, U2, rec.normal);
            double pdf=dot(rec.normal,L.normalize())/pi;
            if(pdf<0.001f)pdf=0.001f;
            scattered = ray(rec.p, L);
            attenuation = albedo/pdf/5;*/
            vec3 scatter_direction = rec.normal + random_unit_vector();
            if(random_double(0,1)<0.3){
                scatter_direction = vec3(0,19.9,-15)-rec.p;
                scattered = ray(rec.p, scatter_direction);
                attenuation = albedo * dot(scattered.direction(), rec.normal)/80;
            }
            else{
                scattered = ray(rec.p, scatter_direction);
                attenuation = albedo * dot(scattered.direction(), rec.normal)/4;
            }
            return true;
        }
    //virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

class metal :public material {
public:
    color albedo;
    double fuzz;
public:
    metal(const color& a, double f) :albedo(a), fuzz(f < 1 ? f : 1) { isMetal = true;}
    vec3 reflect(const vec3& v, const vec3& n) const{
        return v - 2 * dot(v, n) * n;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    //virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
};

class dielectric :public material {
public:
    dielectric(double ri) :ref_idx(ri) { }
    vec3 reflect(const vec3& v, const vec3& n) const{
        return v - 2 * dot(v, n) * n;
    }
    vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) const{
        auto cos_theta = dot(-uv, n);
        vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
        return r_out_perp + r_out_parallel;
    }
    double schlick(double cosine, double ref_idx) const{
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 *= r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        //判断交点是在外部还是内部 front_face为true时-->外部
        double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;
        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        if (etai_over_etat * sin_theta > 1.0) { //全内反射
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        double reflect_prob = schlick(cos_theta, etai_over_etat);//反射率
        if (random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }
public:
    double ref_idx;
};

class lambertianChess :public material {
public:
    shared_ptr<chessTexture> tex;
public:
    lambertianChess(const shared_ptr<chessTexture> a) :tex(a) { }
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = tex->getCol(rec.p);
            return true;
        }
};

class lambertianImage :public material{
public:
    shared_ptr<imageTexture> tex;
    double scaleX,scaleY;
public:
    lambertianImage(QString str,double sx,double sy):scaleX(sx),scaleY(sy)
    {
        tex=make_shared<imageTexture>();
        tex->loadImage(str);
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            /*vec3 scatter_direction = rec.normal + random_unit_vector();
            scattered = ray(rec.p, scatter_direction);
            attenuation = tex->getCol(rec.u*scaleX,rec.v*scaleY);*/

            float U1=random_double(0,1),U2=random_double(0,1);
            vec3 L=SampleCosineHemisphere(U1, U2, rec.normal);
            double pdf=dot(rec.normal,L.normalize())/pi;
            if(pdf<0.001f)pdf=0.001f;
            scattered = ray(rec.p, L);
            attenuation = tex->getCol(rec.u*scaleX,rec.v*scaleY)/pdf/4;
            /*disney dis;
            attenuation = dis.BRDF(vec3(0,10,0)-rec.p,camera_pos-rec.p,rec.normal,
                               r_in.direction(),L.normalize(),tex->getCol(rec.u*scaleX,rec.v*scaleY))/pdf*3;*/
            return true;
        }
};

class light :public material{
public:
    color lightColor;
    float itensity;
    light(color c,float i):lightColor(c),itensity(i){
        isLight = true;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = lightColor * itensity;
        return true;
    }
};

#endif // ! MATERIAL_H
