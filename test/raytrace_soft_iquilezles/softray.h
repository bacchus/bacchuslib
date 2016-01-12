// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// A list of usefull distance function to simple primitives, and an example on how to
// do some interesting boolean operations, repetition and displacement.
// More info here: http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

// bacchus: demo running on cpu

#pragma once

#include "math/mdefs.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat2.h"
#include "math/mat3.h"
#include "utils/timer.h"

namespace bacchus {

//template<typename T>
//inline T mix(const T& x, const T& y, float a) {
//    return (1.f - a)*x + a*y;
//}

//template<typename T>
//inline T smoothstep(const T& x, const T& y, float a) {
//    T t = clamp((T(a) - x)/(y - x), 0.f, 1.f);
//    return 3.f*t*t - 2.f*t*t*t;
//}

//inline vec3f reflect(const vec3f& I, const vec3f& N) {
//    return I - 2.f * dot(N,I) * N;
//}

//inline vec3f sin(const vec3f& v1) {
//    return vec3f(std::sin(v1.x),
//                 std::sin(v1.y),
//                 std::sin(v1.z));
//}

//----------------------------------------------------------------------

inline float sdPlane(const vec3f& p) {
    return p.y;
}

inline float sdSphere(const vec3f& p, float s) {
    return length(p)-s;
}

inline float sdBox(const vec3f& p, const vec3f& b) {
    vec3f d = abs(p) - b;
    return std::min( max3(d.x,d.y,d.z), 0.f) + length(max(d,vec3f(0.f)));
}

inline float sdEllipsoid(const vec3f& p, const vec3f& r) {
    return (length(p/r) - 1.f) * min3(r.x,r.y,r.z);
}

inline float udRoundBox(const vec3f& p, const vec3f& b, float r) {
    return length(max(abs(p)-b,vec3f(0.f)))-r;
}

inline float sdTorus(const vec3f& p, const vec2f& t) {
    return length(vec2f(length(vec2f(p.x,p.z))-t.x,p.y))-t.y;
}

inline float sdHexPrism(const vec3f& p, const vec2f& h) {
    vec3f q = abs(p);
    //#if 0
    //    return max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x);
    //#else
    float d1 = q.z-h.y;
    float d2 = std::max(q.x*0.866025f+q.y*0.5f,q.y)-h.x;
    return length(max(vec2f(d1,d2),vec2f(0.f))) + std::min(std::max(d1,d2), 0.f);
    //#endif
}

inline float sdCapsule(const vec3f& p, const vec3f& a, const vec3f& b, float r) {
    vec3f pa = p-a;
    vec3f ba = b-a;
    float h = clamp(dot(pa,ba)/dot(ba,ba), 0.f, 1.f);
    return length(pa - ba*h) - r;
}

inline float sdTriPrism(const vec3f& p, const vec2f& h) {
    vec3f q = abs(p);
    //#if 0
    //    return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
    //#else
    float d1 = q.z-h.y;
    float d2 = std::max(q.x*0.866025f+p.y*0.5f,-p.y)-h.x*0.5f;
    return length(max(vec2f(d1,d2),vec2f(0.f))) + std::min(std::max(d1,d2), 0.f);
    //#endif
}

inline float sdCylinder(const vec3f& p, const vec2f& h) {
    vec2f d = abs(vec2f(length(vec2f(p.x,p.z)),p.y)) - h;
    return std::min(std::max(d.x,d.y),0.f) + length(max(d,vec2f(0.f)));
}

inline float sdCone(const vec3f& p, const vec3f& c) {
    vec2f q = vec2f(length(vec2f(p.x,p.z)), p.y);
    float d1 = -q.y-c.z;
    float d2 = std::max(dot(q,c.xy()), q.y);
    return length(max(vec2f(d1,d2),vec2f(0.f))) + std::min(std::max(d1,d2), 0.f);
}

inline float sdConeSection(const vec3f& p, float h, float r1, float r2) {
    float d1 = -p.y - h;
    float q = p.y - h;
    float si = 0.5*(r1-r2)/h;
    vec2f pxz = vec2f(p.x,p.z);
    float d2 = std::max(std::sqrt(dot(pxz,pxz)*(1.f-si*si)) + q*si - r2, q);
    return length(max(vec2f(d1,d2),vec2f(0.f))) + std::min(std::max(d1,d2), 0.f);
}

//----------------------------------------------------------------------

inline float length2(const vec2f& p) {
    return sqrt(p.x*p.x + p.y*p.y);
}

inline float length6(const vec2f& p) {
    vec2f t = p*p*p; t = t*t;
    return pow(t.x + t.y, 1.0/6.0);
}

inline float length8(const vec2f& p) {
    vec2f t = p*p; t = t*t; t = t*t;
    return pow(t.x + t.y, 1.0/8.0);
}

inline float sdTorus82(const vec3f& p, const vec2f& t) {
    vec2f q = vec2f(length2(vec2f(p.x,p.z))-t.x,p.y);
    return length8(q)-t.y;
}

inline float sdTorus88(const vec3f& p, const vec2f& t) {
    vec2f q = vec2f(length8(vec2f(p.x,p.z))-t.x,p.y);
    return length8(q)-t.y;
}

inline float sdCylinder6(const vec3f& p, const vec2f& h) {
    return std::max(length6(vec2f(p.x,p.z))-h.x, std::abs(p.y)-h.y);
}

//----------------------------------------------------------------------

float opS(float d1, float d2) {
    return std::max(-d2,d1);
}

vec2f opU(const vec2f& d1, const vec2f& d2) {
    return (d1.x < d2.x) ? d1 : d2;
}

vec3f opRep(const vec3f& p, const vec3f& c) {
    return vec3f(mod(p,c)) - 0.5f*c;
}

vec3f opTwist(const vec3f& p) {
    float c = std::cos(10.f*p.y+10.f);
    float s = std::sin(10.f*p.y+10.f);
    mat2f m = mat2f(c,-s,s,c);
    return vec3f(m*vec2f(p.x,p.z),p.y);
}

//----------------------------------------------------------------------
vec2f find_nearest(const vec3f& pos) {
    //ScopedMeanTimer timer("map");
    vec2f res = opU(vec2f(sdPlane(   pos), 1.0),
                    vec2f(sdSphere(  pos-vec3f(0.0,0.25, 0.0), 0.25), 46.9));
    res = opU(res, vec2f(sdBox(      pos-vec3f(1.0,0.25, 0.0), vec3f(0.25)), 3.0));
    res = opU(res, vec2f(udRoundBox( pos-vec3f(1.0,0.25, 1.0), vec3f(0.15), 0.1), 41.0));
    res = opU(res, vec2f(sdTorus(    pos-vec3f(0.0,0.25, 1.0), vec2f(0.20,0.05)), 25.0));
    res = opU(res, vec2f(sdCapsule(  pos,vec3f(-1.3,0.10,-0.1), vec3f(-0.8,0.50,0.2), 0.1), 31.9));
    res = opU(res, vec2f(sdTriPrism( pos-vec3f(-1.0,0.25,-1.0), vec2f(0.25,0.05)),43.5));
    res = opU(res, vec2f(sdCylinder( pos-vec3f(1.0,0.30,-1.0), vec2f(0.1,0.2)), 8.0));
    res = opU(res, vec2f(sdCone(     pos-vec3f(0.0,0.50,-1.0), vec3f(0.8,0.6,0.3)), 55.0));
    res = opU(res, vec2f(sdTorus82(  pos-vec3f(0.0,0.25, 2.0), vec2f(0.20,0.05)),50.0));
    res = opU(res, vec2f(sdTorus88(  pos-vec3f(-1.0,0.25, 2.0), vec2f(0.20,0.05)),43.0));
    res = opU(res, vec2f(sdCylinder6(pos-vec3f(1.0,0.30, 2.0), vec2f(0.1,0.2)), 12.0));
    res = opU(res, vec2f(sdHexPrism( pos-vec3f(-1.0,0.20, 1.0), vec2f(0.25,0.05)),17.0));

    res = opU(res, vec2f(opS(
                             udRoundBox( pos-vec3f(-2.0,0.2, 1.0), vec3f(0.15),0.05),
                             sdSphere(   pos-vec3f(-2.0,0.2, 1.0), 0.25)), 13.0));
    res = opU(res, vec2f(opS(
                             sdTorus82( pos-vec3f(-2.0,0.2, 0.0), vec2f(0.20,0.1)),
                             sdCylinder( opRep(vec3f(atan2(pos.x+2.0,pos.z)/6.2831,pos.y,
                                                     0.02+0.5*length(pos-vec3f(-2.0,0.2, 0.0))),
                                               vec3f(0.05,1.0,0.05)), vec2f(0.02,0.6))), 51.0));
    res = opU(res, vec2f(0.7*sdSphere(   pos-vec3f(-2.0,0.25,-1.0), 0.2) +
                         0.03*std::sin(50.0*pos.x)*std::sin(50.0*pos.y)*std::sin(50.0*pos.z), 65.0));
    res = opU(res, vec2f(0.5*sdTorus(opTwist(pos-vec3f(-2.0,0.25, 2.0)),vec2f(0.20,0.05)), 46.7));
    res = opU(res, vec2f(sdConeSection(pos-vec3f(0.0,0.35,-2.0), 0.15, 0.2, 0.1), 13.67));
    res = opU(res, vec2f(sdEllipsoid(pos-vec3f(1.0,0.35,-2.0), vec3f(0.15, 0.2, 0.05)), 43.17));

    return res;
}

vec2f castRay(const vec3f& ro, const vec3f& rd) {
    float tmin = 1.f;
    float tmax = 20.f;

//#if 0
//    float tp1 = (0.0-ro.y)/rd.y; if(tp1>0.0) tmax = min(tmax, tp1);
//    float tp2 = (1.6-ro.y)/rd.y; if(tp2>0.0) { if(ro.y>1.6) tmin = max(tmin, tp2);
//        else           tmax = min(tmax, tp2); }
//#endif

    float precis = 0.002f;
    float t = tmin;
    float m = -1.f;
    for(int i=0; i<50; ++i) {
        vec2f res = find_nearest(ro+rd*t);
        if(res.x<precis || t>tmax) break;
        t += res.x;
        m = res.y;
    }

    if(t>tmax) m=-1.f;
    return vec2f(t, m);
}

float softshadow(const vec3f& ro, const vec3f& rd, float mint, float tmax) {
    float res = 1.f;
    float t = mint;
    for(int i=0; i<16; i++) {
        float h = find_nearest(ro + rd*t).x;
        res = std::min(res, 8.f*h/t);
        t += clamp(h, 0.02f, 0.10f);
        if(h<0.001f || t>tmax) break;
    }
    return clamp(res, 0.f, 1.f);
}

vec3f calcNormal(const vec3f& pos) {
    float eps = 0.001f;
    vec3f xyy(eps,0.f,0.f);
    vec3f yxy(0.f,eps,0.f);
    vec3f yyx(0.f,0.f,eps);
    vec3f nor = vec3f(find_nearest(pos+xyy).x - find_nearest(pos-xyy).x,
                      find_nearest(pos+yxy).x - find_nearest(pos-yxy).x,
                      find_nearest(pos+yyx).x - find_nearest(pos-yyx).x);
    return normalize(nor);
}

float calcAO(const vec3f& pos, const vec3f& nor) {
    float occ = 0.0;
    float sca = 1.0;
    for(int i=0; i<5; i++) {
        float hr = 0.01f + 0.12f*float(i)/4.f;
        vec3f aopos =  nor * hr + pos;
        float dd = find_nearest(aopos).x;
        occ += -(dd-hr)*sca;
        sca *= 0.95f;
    }
    return clamp(1.f - 3.f*occ, 0.f, 1.f);
}

mat3f setCamera(const vec3f& ro, const vec3f& ta, float cr) {
    vec3f cw = normalize(ta-ro);
    vec3f cp = vec3f(std::sin(cr), std::cos(cr), 0.f);
    vec3f cu = normalize(cross(cw,cp));
    vec3f cv = normalize(cross(cu,cw));
    return mat3f(cu, cv, cw);
}

vec3f render(const vec3f& ro, const vec3f& rd) {
    //ScopedMeanTimer timer("render");// 0,3111 ms
    vec3f col = vec3f(0.8, 0.9, 1.0);
    vec2f res = castRay(ro,rd);
    float t = res.x;
    float m = res.y;
    if(m>-0.5f) {
        vec3f pos = ro + t*rd;
        vec3f nor = calcNormal(pos);
        vec3f ref = reflect(rd, nor);

        // material
        col = vec3f(0.45f) + 0.3f*sin(vec3f(0.05f,0.08f,0.10f)*(m-1.f));

        if(m<1.5) {
            float f = (int)(std::floor(5.f*pos.z) + std::floor(5.f*pos.x)) % 2;
            col = vec3f(0.4f) + 0.1f*f*vec3f(1.f);
        }

        // lighitng
        float occ = calcAO(pos, nor);
        vec3f  lig = normalize(vec3f(-0.6f, 0.7f, -0.5f));
        float amb = clamp(0.5+0.5*nor.y, 0.f, 1.f);
        float dif = clamp(dot(nor, lig), 0.f, 1.f);
        float bac = clamp(dot(nor, normalize(vec3f(-lig.x,0.f,-lig.z))), 0.f, 1.f)*clamp(1.f-pos.y,0.f,1.f);
        float dom = smoothstep(-0.1f, 0.1f, ref.y);
        float fre = pow(clamp(1.0+dot(nor,rd),0.f,1.f), 2.f);
        float spe = pow(clamp(dot(ref, lig), 0.f, 1.f),16.f);

        dif *= softshadow(pos, lig, 0.02f, 2.5f);
        dom *= softshadow(pos, ref, 0.02f, 2.5f);

        vec3f brdf = vec3f(0.0);
        brdf += 1.20f*dif*vec3f(1.00f,0.90f,0.60f);
        brdf += 1.20f*spe*vec3f(1.00f,0.90f,0.60f)*dif;
        brdf += 0.30f*amb*vec3f(0.50f,0.70f,1.00f)*occ;
        brdf += 0.40f*dom*vec3f(0.50f,0.70f,1.00f)*occ;
        brdf += 0.30f*bac*vec3f(0.25f,0.25f,0.25f)*occ;
        brdf += 0.40f*fre*vec3f(1.00f,1.00f,1.00f)*occ;
        brdf += vec3f(0.02f);
        col = col*brdf;
        col = mix(col, vec3f(0.8f,0.9f,1.0f), 1.0f-exp(-0.0005f*t*t));
    }
    return vec3f(clamp(col,0.f,1.f));
}

//const vec4f& fragColor
int mainImage(const vec2f& fragCoord, const vec2f& iResolution, const vec2f& iMouse, float iGlobalTime) {
    //ScopedTimer timer("mainImage");// 0,3111 ms
    vec2f q = fragCoord/iResolution;
    vec2f p = vec2f(-1.f) + 2.f*q;
    p.x *= iResolution.x/iResolution.y;
    vec2f mo = iMouse/iResolution;

    float time = 15.0 + iGlobalTime;

    // camera
    vec3f ro = vec3f(-0.5f + 3.5f*std::cos(0.1f*time + 6.0f*mo.x),
                      1.0f + 2.0f*mo.y,
                      0.5f + 3.5f*std::sin(0.1f*time + 6.0f*mo.x));
    vec3f ta = vec3f(-0.5, -0.4, 0.5);

    // camera-to-world transformation
    mat3f ca = setCamera(ro, ta, 0.0);

    // ray direction
    vec3f rd = ca * normalize(vec3f(p.x,p.y,2.f));

    // render
    vec3f col = render(ro, rd);

//    col = vec3f(pow(col.x,0.4545),
//                pow(col.y,0.4545),
//                pow(col.z,0.4545));
//    fragColor = vec4f(col, 1.f);

    return rgba2i(255.f*pow(col.x,0.4545),
                  255.f*pow(col.y,0.4545),
                  255.f*pow(col.z,0.4545));
}

} // namespace bacchus
