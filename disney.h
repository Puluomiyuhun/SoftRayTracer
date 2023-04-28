#ifndef DISNEY_H
#define DISNEY_H
#include"vec3.h"
#include"rtweekend.h"

class disney
{
private:
    float metallic = 1;
    float subsurface = 0;
    float specular = 0.5;
    float roughness = 0.5;
    float specularTint = 0;
    float anisotropic = 0;
    float sheen = 0;
    float sheenTint = .5;
    float clearcoat = 0;
    float clearcoatGloss = 1;

    float sqr(float x) { return x*x; }

    float SchlickFresnel(float u)
    {
        float m = clamp(1-u, 0, 1);
        float m2 = m*m;
        return m2*m2*m; // pow(m,5)
    }

    float GTR1(float NdotH, float a)
    {
        if (a >= 1) return 1/pi;
        float a2 = a*a;
        float t = 1 + (a2-1)*NdotH*NdotH;
        return (a2-1) / (pi*log(a2)*t);
    }

    float GTR2(float NdotH, float a)
    {
        float a2 = a*a;
        float t = 1 + (a2-1)*NdotH*NdotH;
        return a2 / (pi * t*t);
    }

    float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
    {
        return 1 / (pi * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));
    }

    float smithG_GGX(float NdotV, float alphaG)
    {
        float a = alphaG*alphaG;
        float b = NdotV*NdotV;
        return 1 / (NdotV + sqrt(a + b - a*b));
    }

    float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
    {
        return 1 / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));
    }

    vec3 mon2lin(vec3 x)
    {
        return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
    }

    float mix(float a,float b,float c){
        return (1-c)*a+c*b;
    }

    vec3 mix(vec3 a,vec3 b,float c){
        return (1-c)*a+c*b;
    }

public:
    disney(){};
    vec3 BRDF( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y,color baseColor)
    {
        float NdotL = dot(N,L);
        float NdotV = dot(N,V);
        if (NdotL < 0 || NdotV < 0) return vec3(0,0,0);

        vec3 H = (L+V).normalize();
        float NdotH = dot(N,H);
        float LdotH = dot(L,H);

        vec3 Cdlin = mon2lin(baseColor);
        float Cdlum = .3*Cdlin[0] + .6*Cdlin[1]  + .1*Cdlin[2]; // luminance approx.

        vec3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : vec3(1,1,1); // normalize lum. to isolate hue+sat
        vec3 Cspec0 = mix(specular*.08*mix(vec3(1,1,1), Ctint, specularTint), Cdlin, metallic);
        vec3 Csheen = mix(vec3(1,1,1), Ctint, sheenTint);

        // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
        // and mix in diffuse retro-reflection based on roughness
        float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
        float Fd90 = 0.5 + 2 * LdotH*LdotH * roughness;
        float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

        // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
        // 1.25 scale is used to (roughly) preserve albedo
        // Fss90 used to "flatten" retroreflection based on roughness
        float Fss90 = LdotH*LdotH*roughness;
        float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
        float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

        // specular
        float aspect = sqrt(1-anisotropic*.9);
        float ax = maxd(.001, sqr(roughness)/aspect);
        float ay = maxd(.001, sqr(roughness)*aspect);
        float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
        float FH = SchlickFresnel(LdotH);
        vec3 Fs = mix(Cspec0, vec3(1,1,1), FH);
        float Gs;
        Gs  = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
        Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

        // sheen
        vec3 Fsheen = FH * sheen * Csheen;

        // clearcoat (ior = 1.5 -> F0 = 0.04)
        float Dr = GTR1(NdotH, mix(.1,.001,clearcoatGloss));
        float Fr = mix(.04, 1.0, FH);
        float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);

        return ((1/pi) * mix(Fd, ss, subsurface)*Cdlin + Fsheen)
            * (1-metallic)
            + Gs*Fs*Ds + .25*vec3(clearcoat,clearcoat,clearcoat)*Gr*Fr*Dr;
    }
};

class ggx{
private:
    float alpha = 0.1;
public:
    ggx(){}
    float sqr(float x) { return x*x; }

    float GGX(float alpha, float cosThetaM)
    {
        float CosSquared = cosThetaM*cosThetaM;
        float TanSquared = (1-CosSquared)/CosSquared;
        return (1.0/pi) * sqr(alpha/(CosSquared * (alpha*alpha + TanSquared)));
    }

    vec3 BRDF( vec3 L, vec3 V, vec3 N)
    {
        vec3 H = ( L + V ).normalize();
        float D = GGX(alpha, dot(N,H));
        return vec3(D,D,D);
    }
};

#endif // DISNEY_H
