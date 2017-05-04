#pragma once

#include <vector>

#include "math/mdefs.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat2.h"
#include "math/mat3.h"

#include "image/image.h"

#include "utils/timer.h"

namespace bacchus {

//--------------------------------------------------------------------
struct Mtl {
    vec3f col;
};

struct Prim {
    Mtl mtl;
    virtual float sd(const vec3f& pos) = 0;
};

struct Plane: public Prim {
    vec3f normal;
    virtual float sd(const vec3f &pos) override {
        return pos.y;
    }
};

struct Sphere: public Prim {
    vec3f center;
    float radius;
    virtual float sd(const vec3f &pos) override {
        return length(pos - center) - radius;
    }
};

//--------------------------------------------------------------------
std::vector<Prim*> prims;

void init_scene() {
    Plane* p1 = new Plane;
    p1->mtl.col = vec3f(0.8f,0.5f,0.2f);
    prims.push_back(p1);

    for (int i = -2; i < 3; ++i) {
        for (int j = -2; j < 3; ++j) {

            Sphere* p2 = new Sphere;
            p2->center = vec3f(i*0.5f, 0.25f, j*0.5f);
            p2->radius = 0.2f;
            p2->mtl.col = vec3f(0.2,0.7,0.2);
            prims.push_back(p2);
        }
    }

}

//--------------------------------------------------------------------
mat3f setCamera(const vec3f& ro, const vec3f& ta
                , float rol = 0, float fov = 1) {
    vec3f zax = normalize(ta-ro);
    vec3f up = vec3f(std::sin(rol), std::cos(rol), 0.f);
    vec3f xax = fov*normalize(cross(zax,up));
    vec3f yax = fov*normalize(cross(xax,zax));
    return mat3f(xax, yax, zax);
}

//--------------------------------------------------------------------
Prim* find_nearest(const vec3f& pos, float& d) {
    Prim* res = nullptr;
    d = BCC_M_FLOAT_MAX;
    for (Prim* p: prims) {
        float t = p->sd(pos);
        if (t < d) {
            d = t;
            res = p;
        }
    }
    return res;
}

//--------------------------------------------------------------------
vec3f calcNormal(const vec3f& pos) {
    float eps = 0.001f;
    vec3f xyy(eps,0.f,0.f);
    vec3f yxy(0.f,eps,0.f);
    vec3f yyx(0.f,0.f,eps);

    vec3f t[2];

    find_nearest(pos+xyy, t[0].x);
    find_nearest(pos-xyy, t[1].x);

    find_nearest(pos+yxy, t[0].y);
    find_nearest(pos-yxy, t[1].y);

    find_nearest(pos+yyx, t[0].z);
    find_nearest(pos-yyx, t[1].z);

    return normalize(t[0] - t[1]);
}

//--------------------------------------------------------------------
Prim* castRay(const vec3f& rori, const vec3f& rdir, float& dist) {
    const int maxsteps = 50;
    const float tmin = 1.f;
    const float tmax = 20.f;
    const float precis = 0.002f;

    dist = tmin;
    float t = 0.0f;
    Prim* m = nullptr;

    for (int i = 0; i < maxsteps; ++i) {
        Prim* resm = find_nearest(rori + rdir*dist, t);
        if (t < precis || dist > tmax)
            break;
        dist += t;
        m = resm;
    }

    return (dist > tmax ? nullptr : m);
}

//--------------------------------------------------------------------
vec3f render_ray(const vec3f& eye, const vec3f& dir) {
    const vec3f bgcolor = vec3f(0.8, 0.9, 1.0);
    const vec3f ligdir = vec3f(-0.6f, 0.7f, -0.5f);

    vec3f col = bgcolor;
    float t;
    Prim* p = castRay(eye,dir, t);
    if(p) {
        col = p->mtl.col;
        const vec3f pos = eye + t*dir;
        const vec3f nor = calcNormal(pos);
        const vec3f lig = normalize(ligdir);
        const float dif = clamp(dot(nor,lig), 0.f, 1.f);
        vec3f brdf = dif*vec3f(1.0f,0.9f,0.6f);
        col = col*brdf;
        col = mix(col, bgcolor, 1.0f - exp(-0.005f*t*t));// fog
    }
    return clamp(col,0.f,1.f);
}

//--------------------------------------------------------------------
void process_scene(Image& img) {
    ScopedTimer timer("mytrace");
    init_scene();
    vec3f eye = vec3f(1.0f, 5.0f, 5.0f);
    vec3f tar = vec3f(0.0f, 0.0f, 0.0f);
    mat3f view = setCamera(eye, tar);

    vec2i fcord = 0.5f*img.resolution();
    for (fcord.y = 0; fcord.y < img.resolution().y; ++fcord.y) {
        for (fcord.x = 0; fcord.x < img.resolution().x; ++fcord.x) {
            vec2f q = vec2f(fcord)/vec2f(img.resolution());
            vec2f p = 2.f*q - vec2f(1.f);
            vec3f raydir = view * normalize(vec3f(p,2.f));
            img.pix(fcord) = rgb2i(255.f*render_ray(eye, raydir));
        }
    }
}

} // namespace bcc
