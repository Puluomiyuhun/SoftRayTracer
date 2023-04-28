#include "triangle.h"
//#include "vec3.h"
bool triangle::SameSide(vec3 A, vec3 B, vec3 C, vec3 P)const
{
    vec3 AB = B - A ;
    vec3 AC = C - A ;
    vec3 AP = P - A ;

    vec3 v1 = cross(AB,AC) ;
    vec3 v2 = cross(AB,AP) ;

    return dot(v1,v2) >= 0 ;
}

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    /*n.(p0- porig)/ n.vDir*/
    //if(dot(normal,r.direction())==0)return false;
    double t=dot(normal,pt[0]-r.origin())/dot(normal,r.direction());

    if (t > t_min&& t < t_max)
    {
        point3 P=r.at(t);
        if(SameSide(pt[0],pt[1],pt[2],P) && SameSide(pt[1],pt[2],pt[0],P)
                &&SameSide(pt[2],pt[0],pt[1],P))
        {
            rec.t=t;
            rec.p=r.at(t);
            rec.set_face_normal(r,normal);
            rec.mat_ptr=mat_ptr;
            /*求uv*/
            /*double tmp;vec3 T1,T2;
            if(u[0]!=u[2])
            {
                tmp=(u[1]-u[0])/(u[0]-u[2]);
                T1=(pt[2]-pt[0])*tmp+(pt[1]-pt[0]);
            }
            else T1=pt[0]-pt[2];
            if(v[0]!=v[2])
            {
                tmp=(v[1]-v[0])/(v[0]-v[2]);
                T2=(pt[2]-pt[0])*tmp+(pt[1]-pt[0]);
            }
            else T2=pt[0]-pt[2];
            if(dot(T2,(pt[1]-pt[0]))!=0&&u[1]-u[0]!=0)
                T2=unit_vector(T2)*(dot(T2,(pt[1]-pt[0]))/T2.length())/(u[1]-u[0]);
            else if(dot(T2,(pt[2]-pt[0]))!=0&&u[2]-u[0]!=0)
                T2=unit_vector(T2)*(dot(T2,(pt[2]-pt[0]))/T2.length())/(u[2]-u[0]);
            else if(dot(T2,(pt[2]-pt[1]))!=0&&u[2]-u[1]!=0)
                T2=unit_vector(T2)*(dot(T2,(pt[2]-pt[1]))/T2.length())/(u[2]-u[1]);

            printf("%lf %lf\n",(pt[1]-pt[0]).length(),v[1]-v[0]);
            if(dot(T1,(pt[1]-pt[0]))!=0&&v[1]-v[0]!=0)
                T1=unit_vector(T1)*(dot(T1,(pt[1]-pt[0]))/T1.length())/(v[1]-v[0]);
            else if(dot(T1,(pt[2]-pt[0]))!=0&&v[2]-v[0]!=0)
                T1=unit_vector(T1)*(dot(T1,(pt[2]-pt[0]))/T1.length())/(v[2]-v[0]);
            else if(dot(T1,(pt[2]-pt[1]))!=0&&v[2]-v[1]!=0)
                T1=unit_vector(T1)*(dot(T1,(pt[2]-pt[1]))/T1.length())/(v[2]-v[1]);

            vec3 Ori=pt[0]-u[0]*T2-v[0]*T1;
            rec.p=rec.p-Ori;
            if(T1.x()*T2.y()-T2.x()*T1.y()!=0)
            {
                rec.u=(T1.x()*rec.p.y()-T1.y()*rec.p.x())/(T1.x()*T2.y()-T2.x()*T1.y());
                rec.v=(T2.x()*rec.p.y()-T2.y()*rec.p.x())/(T2.x()*T1.y()-T1.x()*T2.y());
            }
            else if(T1.x()*T2.z()-T2.x()*T1.z()!=0)
            {
                rec.u=(T1.x()*rec.p.z()-T1.z()*rec.p.x())/(T1.x()*T2.z()-T2.x()*T1.z());
                rec.v=(T2.x()*rec.p.z()-T2.z()*rec.p.x())/(T2.x()*T1.z()-T1.x()*T2.z());
            }
            else if(T1.y()*T2.z()-T2.y()*T1.z()!=0)
            {
                rec.u=(T1.y()*rec.p.z()-T1.z()*rec.p.y())/(T1.y()*T2.z()-T2.y()*T1.z());
                rec.v=(T2.y()*rec.p.z()-T2.z()*rec.p.y())/(T2.y()*T1.z()-T1.y()*T2.z());
            }
            rec.p=rec.p+Ori;*/
            vec3 V1,V2,Vp;
            double p,q;
            Vp=rec.p-pt[0];
            V1=pt[1]-pt[0];
            V2=pt[2]-pt[0];
            if(V2.x()*V1.y()!=V1.x()*V2.y())
            {
                p=(V2.x()*Vp.y()-V2.y()*Vp.x())/(V2.x()*V1.y()-V1.x()*V2.y());
                q=(V1.x()*Vp.y()-V1.y()*Vp.x())/(V1.x()*V2.y()-V2.x()*V1.y());
            }
            else if(V2.x()*V1.z()!=V1.x()*V2.z())
            {
                p=(V2.x()*Vp.z()-V2.z()*rec.p.x())/(V2.x()*V1.z()-V1.x()*V2.z());
                q=(V1.x()*Vp.z()-V1.z()*rec.p.x())/(V1.x()*V2.z()-V2.x()*V1.z());
            }
            else
            {
                p=(V2.y()*rec.p.z()-V2.z()*rec.p.y())/(V2.y()*V1.z()-V1.y()*V2.z());
                q=(V1.y()*rec.p.z()-V1.z()*rec.p.y())/(V1.y()*V2.z()-V2.y()*V1.z());
            }
            rec.u=u[0]+p*(u[1]-u[0])+q*(u[2]-u[0]);
            rec.v=v[0]+p*(v[1]-v[0])+q*(v[2]-v[0]);
            return true;
        }
    }
    return false;
}
