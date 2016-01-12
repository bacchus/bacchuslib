// from here: http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
// by Inigo Quilez
// adopted to c++ software render

#pragma once

#include "math/mdefs.h"
#include "math/vec3.h"

namespace bacchus {

float dot2(vec3f v) {
    return dot(v,v);
}

// primitives -----------------------------------------------------------
// All primitives are centered at the origin. You will have to transform
// the point to get arbitrarily rotated, translated and scaled objects

float sdSphere(vec3f p, float s) {
    return length(p)-s;
}

float udBox(vec3f p, vec3f b) {
    return length(max(abs(p)-b,0.0));
}

float udRoundBox(vec3f p, vec3f b, float r) {
    return length(max(abs(p)-b,0.0))-r;
}

float sdBox(vec3f p, vec3f b) {
    vec3f d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) +
            length(max(d,0.0));
}

float sdTorus(vec3f p, vec2 t) {
    vec2 q = vec2(length(p.xz)-t.x,p.y);
    return length(q)-t.y;
}

float sdCylinder(vec3f p, vec3f c) {
    return length(p.xz-c.xy)-c.z;
}

float sdCone(vec3f p, vec2 c) {
    // c must be normalized
    float q = length(p.xy);
    return dot(c,vec2(q,p.z));
}

float sdPlane(vec3f p, vec4 n) {
    // n must be normalized
    return dot(p,n.xyz) + n.w;
}

float sdHexPrism(vec3f p, vec2 h) {
    vec3f q = abs(p);
    return max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x);
}

float sdTriPrism(vec3f p, vec2 h) {
    vec3f q = abs(p);
    return max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5);
}

float sdCapsule(vec3f p, vec3f a, vec3f b, float r) {
    vec3f pa = p - a, ba = b - a;
    float h = clamp(dot(pa,ba)/dot(ba,ba), 0.0, 1.0);
    return length(pa - ba*h) - r;
}

float sdCappedCylinder(vec3f p, vec2 h) {
    vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdCappedCone(vec3f p, vec3f c) {
    vec2 q = vec2(length(p.xz), p.y);
    vec2 v = vec2(c.z*c.y/c.x, -c.z);
    vec2 w = v - q;
    vec2 vv = vec2(dot(v,v), v.x*v.x);
    vec2 qv = vec2(dot(v,w), v.x*w.x);
    vec2 d = max(qv,0.0)*qv/vv;
    return sqrt(dot(w,w) - max(d.x,d.y)) *
            sign(max(q.y*v.x-q.x*v.y,w.y));
}

float sdEllipsoid(vec3f p, vec3f r) {
    return (length(p/r) - 1.0) * min(min(r.x,r.y),r.z);
}

float udTriangle(vec3f p, vec3f a, vec3f b, vec3f c) {
    vec3f ba = b - a; vec3f pa = p - a;
    vec3f cb = c - b; vec3f pb = p - b;
    vec3f ac = a - c; vec3f pc = p - c;
    vec3f nor = cross(ba, ac);

    return sqrt(
                (sign(dot(cross(ba,nor),pa)) +
                 sign(dot(cross(cb,nor),pb)) +
                 sign(dot(cross(ac,nor),pc)) < 2.0)
                ?
                    min(min(
                            dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),
                            dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb)),
                        dot2(ac*clamp(dot(ac,pc)/dot2(ac),0.0,1.0)-pc))
                  :
                    dot(nor,pa)*dot(nor,pa)/dot2(nor));
}

float udQuad(vec3f p, vec3f a, vec3f b, vec3f c, vec3f d) {
    vec3f ba = b - a; vec3f pa = p - a;
    vec3f cb = c - b; vec3f pb = p - b;
    vec3f dc = d - c; vec3f pc = p - c;
    vec3f ad = a - d; vec3f pd = p - d;
    vec3f nor = cross(ba, ad);

    return sqrt(
                (sign(dot(cross(ba,nor),pa)) +
                 sign(dot(cross(cb,nor),pb)) +
                 sign(dot(cross(dc,nor),pc)) +
                 sign(dot(cross(ad,nor),pd)) < 3.0)
                ?
                    min(min(min(
                                dot2(ba*clamp(dot(ba,pa)/dot2(ba),0.0,1.0)-pa),
                                dot2(cb*clamp(dot(cb,pb)/dot2(cb),0.0,1.0)-pb)),
                            dot2(dc*clamp(dot(dc,pc)/dot2(dc),0.0,1.0)-pc)),
                        dot2(ad*clamp(dot(ad,pd)/dot2(ad),0.0,1.0)-pd))
                  :
                    dot(nor,pa)*dot(nor,pa)/dot2(nor));
}

// Most of these functions can be modified to use other norms than the
// euclidean. By replacing length(p), which computes
// (x^2+y^2+z^2)^(1/2) by (x^n+y^n+z^n)^(1/n) one can get variations
// of the basic primitives that have rounded edges rather than sharp ones
// examples:

float sdTorus82(vec3f p, vec2 t) {
    vec2 q = vec2(length2(p.xz)-t.x,p.y);
    return length8(q)-t.y;
}

float sdTorus88(vec3f p, vec2 t) {
    vec2 q = vec2(length8(p.xz)-t.x,p.y);
    return length8(q)-t.y;
}

// distance operations --------------------------------------------------
// The d1 and d2 parameters in the following functions are the distance
//  to the two distance fields to combine together

// Union
float opU(float d1, float d2) {
    return min(d1,d2);
}

// Substraction
float opS(float d1, float d2) {
    return max(-d1,d2);
}

// Intersection
float opI(float d1, float d2) {
    return max(d1,d2);
}

// smooth distance operations -------------------------------------------

// exponential smooth min (k = 32);
float smin_exp(float a, float b, float k) {
    float res = exp(-k*a) + exp(-k*b);
    return -log(res)/k;
}

// polynomial smooth min (k = 0.1);
float smin_pol(float a, float b, float k) {
    float h = clamp(0.5 + 0.5*(b-a)/k, 0.0, 1.0);
    return mix(b, a, h) - k*h*(1.0-h);
}

// power smooth min (k = 8);
float smin_pow(float a, float b, float k) {
    a = pow(a, k); b = pow(b, k);
    return pow((a*b)/(a+b), 1.0/k);
}

// domain operations ----------------------------------------------------
// Repetition
float opRep(vec3f p, vec3f c) {
    vec3f q = mod(p,c)-0.5*c;
    return primitve(q);
}
// Rotation/Translation
vec3f opTx(vec3f p, mat4f m) {
    vec3f q = invert(m)*p;
    return primitive(q);
}

// Scale
float opScale(vec3f p, float s) {
    return primitive(p/s)*s;
}

// distance deformations ------------------------------------------------

float opDisplace(vec3f p) {
    float d1 = primitive(p);
    float d2 = displacement(p);
    return d1+d2;
}


float opBlend(vec3f p) {
    float d1 = primitiveA(p);
    float d2 = primitiveB(p);
    return smin(d1, d2);
}

// domain deformations --------------------------------------------------

float opTwist(vec3f p) {
    float c = cos(20.0*p.y);
    float s = sin(20.0*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3f  q = vec3f(m*p.xz,p.y);
    return primitive(q);
}

float opCheapBend(vec3f p) {
    float c = cos(20.0*p.y);
    float s = sin(20.0*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3f  q = vec3f(m*p.xy,p.z);
    return primitive(q);
}

} // namespace bacchus
