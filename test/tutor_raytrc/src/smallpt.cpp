// smallpt, a Path Tracer by Kevin Beason, 2008
// Make : g++ -O3 -fopenmp smallpt.cpp -o smallpt
//        Remove "-fopenmp" for g++ version < 4.2
// Usage: time ./smallpt 5000 && xv image.ppm

#include "math/vec3.h"
#include "image/image.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace bacchus;

typedef vec3<double> Vec;
typedef vec2<double> Vec2;

struct Ray {
    Vec o, d;
    Ray(Vec o_, Vec d_) : o(o_), d(d_) {}
};

enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance()

struct Sphere {
    double rad;       // radius
    Vec p, e, c;      // position, emission, color
    Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive)

    Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_):
        rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {}

    // returns distance, 0 if nohit
    double intersect(const Ray &r) const {
        // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
        Vec op = p-r.o;
        double eps = 1e-4;
        double b = dot(op, r.d);
        double det = b*b - dot(op, op) + rad*rad;

        if (det<0)
            return 0;
        else
            det = sqrt(det);

        double t1 = b - det;
        double t2 = b + det;
        return t1 > eps ? t1 : (t2 > eps ? t2 : 0);
    }
};

Sphere spheres[] = {//Scene: radius, position, emission, color, material
                    Sphere(1e5, Vec( 1e5+1,40.8,81.6), Vec(),Vec(.75,.25,.25),DIFF),//Left
                    Sphere(1e5, Vec(-1e5+99,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF),//Rght
                    Sphere(1e5, Vec(50,40.8, 1e5),     Vec(),Vec(.75,.75,.75),DIFF),//Back
                    Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(),Vec(),           DIFF),//Frnt
                    Sphere(1e5, Vec(50, 1e5, 81.6),    Vec(),Vec(.75,.75,.75),DIFF),//Botm
                    Sphere(1e5, Vec(50,-1e5+81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF),//Top
                    Sphere(16.5,Vec(27,16.5,47),       Vec(),Vec(1,1,1)*.999, SPEC),//Mirr
                    Sphere(16.5,Vec(73,16.5,78),       Vec(),Vec(1,1,1)*.999, REFR),//Glas
                    Sphere(600, Vec(50,681.6-.27,81.6),Vec(12,12,12),  Vec(), DIFF) //Lite
                   };

inline int toInt(double x) {
    return int(pow(clamp(x),1/2.2)*255+.5);
}

inline bool intersect(const Ray &r, double &t, int &id) {
    double n = sizeof(spheres)/sizeof(Sphere);
    double d;
    double inf = t = 1e20;

    for (int i = int(n); i--;) {
        if ((d = spheres[i].intersect(r)) && d < t) {
            t = d;
            id = i;
        }
    }
    return t < inf;
}

Vec radiance(const Ray &r, int depth, unsigned short *Xi) {
    double t;                               // distance to intersection
    int id = 0;                             // id of intersected object
    if (!intersect(r, t, id))
        return Vec(); // if miss, return black

    const Sphere& obj = spheres[id];        // the hit object

    Vec x = r.o + r.d*t;
    Vec n = normalize(x - obj.p);
    Vec nl = dot(n, r.d) < 0 ? n : n*-1, f = obj.c;
    double p = max(f); // max refl

    if (++depth > 5) {
        if (erand48(Xi) < p)
            f = f*(1/p);
        else
            return obj.e; //R.R.
    }

    if (obj.refl == DIFF) {                  // Ideal DIFFUSE reflection
        double r1 = 2*M_PI*erand48(Xi);
        double r2 = erand48(Xi);
        double r2s = sqrt(r2);

        Vec w = nl;
        Vec u = normalize(cross((fabs(w.x) > 0.1 ? Vec(0,1,0) : Vec(1,0,0)),w));
        Vec v = cross(w,u);
        Vec d = normalize(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2));

        return obj.e + f*(radiance(Ray(x,d), depth, Xi));

    } else if (obj.refl == SPEC) {           // Ideal SPECULAR reflection
        return obj.e + f*(radiance(Ray(x, r.d - n*2*dot(n, r.d)), depth, Xi));
    }

    Ray reflRay(x, r.d - n*2*dot(n,r.d));     // Ideal dielectric REFRACTION
    bool into = dot(n,nl) > 0;                // Ray from outside going in?
    double nc = 1;
    double nt=1.5;
    double nnt = into ? nc/nt : nt/nc;
    double ddn = dot(r.d,nl);

    double cos2t = 1 - nnt*nnt*(1 - ddn*ddn);
    if (cos2t < 0)                          // Total internal reflection
        return obj.e + f*(radiance(reflRay, depth, Xi));

    Vec tdir = normalize(r.d*nnt - n*((into ? 1 : -1)*(ddn*nnt+sqrt(cos2t))));

    double a = nt - nc;
    double b = nt + nc;
    double R0 = a*a/(b*b);
    double c = 1 - (into ? -ddn : dot(tdir,n));

    double Re = R0 + (1 - R0)*c*c*c*c*c;
    double Tr = 1 - Re;
    double P = 0.25 + 0.5*Re;
    double RP = Re/P;
    double TP = Tr/(1-P);

    // Russian roulette
    return obj.e + f*(depth > 2
                      ? (erand48(Xi) < P
                         ? radiance(reflRay, depth, Xi)*RP
                         : radiance(Ray(x,tdir), depth, Xi)*TP)
                      : radiance(reflRay,depth,Xi)*Re+radiance(Ray(x,tdir),depth,Xi)*Tr);
}

int mainImage(Image& img, int samps) {

    /// setup image
    Vec* c = new Vec[img.width*img.height]; // image

    /// setup camera
    Ray cam(Vec(50,52,295.6), normalize(Vec(0,-0.042612,-1))); // cam (pos, dir)
    // 0.5135 defines fov
    Vec cx = Vec(img.width*0.5135/img.height, 0, 0);             // x dir inc
    Vec cy = normalize(cross(cx,cam.d))*0.5135; // y dir inc

    samps /= 4; // # samples


#pragma omp parallel for schedule(dynamic, 1) private(r)       // OpenMP

    Vec2 xy;
    for (xy.y = 0; xy.y < img.height; xy.y++) {                       // Loop over image rows
        fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samps*4,100.*xy.y/(img.height-1)); // progress

        unsigned short Xi[3] = {0, 0, xy.y*xy.y*xy.y};
        for (xy.x = 0; xy.x < img.width; xy.x++) {   // Loop cols
            const int i = (img.height - xy.y - 1)*img.width + xy.x;
            Vec2 sxy;
            for (sxy.y = 0; sxy.y < 2; ++sxy.y) {          // 2x2 subpixel rows
                for (sxy.x = 0; sxy.x < 2; ++sxy.x) {        // 2x2 subpixel cols

                    Vec r; // used for sample colors
                    for (int s = 0; s < samps; ++s) {
                        /// tent filter
                        const Vec2 r12(2*erand48(Xi), 2*erand48(Xi));
                        const Vec2 dxy = ifv(gtv(Vec2(1),r12), sqrtv(r12) - Vec2(1), Vec2(1) - sqrtv(Vec2(2) - r12));
                        const Vec2 tmp = (0.5*(sxy + Vec2(0.5) + dxy) + xy)/Vec2(img.width,img.height) - Vec2(0.5);
                        const Vec d = cx*tmp.x + cy*tmp.y + cam.d;

                        r = r + radiance(Ray(cam.o + d*140, normalize(d)), 0, Xi)*(1.0/samps);
                    } // Camera rays are pushed ^^^^^ forward to start in interior

                    c[i] = c[i] + Vec(clamp(r.x),clamp(r.y),clamp(r.z))*.25;
                }
            }
        }
    }
//    FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
//    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i=0; i<img.width*img.height; i++) {
        img.dat8[i*4+0] = toInt(c[i].x);
        img.dat8[i*4+1] = toInt(c[i].y);
        img.dat8[i*4+2] = toInt(c[i].z);
        img.dat8[i*4+3] = 255;
//        fprintf(f,"%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
    }
}
