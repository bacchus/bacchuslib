#include <array>

#include "raytracer_devmaster.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "math/transform.h"
#include "utils/timer.h"
#include "image/tga_rw.h"

namespace bacchus {
/*
vpc = c - p  // this is the vector from p to c
if ((vpc . d) < 0)  // when the sphere is behind the origin p
                    // note that this case may be dismissed if it is
                    // considered that p is outside the sphere
    if (|vpc| > r)
        // there is no intersection
    else if (|vpc| == r)
        intersection = p
    else // occurs when p is inside the sphere
        pc = projection of c on the line
        // distance from pc to i1
        dist = sqrt(radius^2 - |pc - c|^2)
        di1 = dist - |pc - p|
        intersection = p + d * di1

else // center of sphere projects on the ray
    pc = projection of c on the line

    if (| c - pc | > r)
        // there is no intersection
    else
        // distance from pc to i1
        dist = sqrt(radius^2 - |pc - c|^2)
        if (|vpc| > r) // origin is outside sphere
            di1 = |pc - p| - dist
        else // origin is inside sphere
            di1 = |pc - p| + dist
        intersection = p + d * di1
*/

vec3f Sphere::normal(const vec3f &pt) const {
    return (pt - center)/radius;
}

eSect Sphere::intersect(const Ray &ray, float &dist) const {
    last_rayid = ray.id;
    vec3f vpc = center - ray.point;
    float b = dot(vpc, ray.dir);
    float det = sq(b) - dot(vpc,vpc) + sq(radius);
    eSect res = eSect::MIS;
    //TODO: omg float 0
    if (det > 0.f) {
        det = sqrt(det);
        float i1 = b - det;
        float i2 = b + det;
        if (i2 > 0) {
            if (i1 < 0) {
                if (i2 < dist) {
                    dist = i2;
                    res = eSect::INS;
                }
            } else {
                if (i1 < dist) {
                    dist = i1;
                    res = eSect::HIT;
                }
            }
        }
    }
    return res;
}

bool Sphere::intersect(const aabb &box) const {
    float dmin = 0.f;
    vec3f v1 = box.posi; vec3f v2 = v1 + box.size;
    for (int i = 0; i < 3; ++i) {
        if      (center[i] < v1[i]) dmin += sq(center[i] - v1[i]);
        else if (center[i] > v2[i]) dmin += sq(center[i] - v2[i]);
    }
    return (dmin <= sq(radius));
}

aabb Sphere::get_aabb() const {
    vec3f size(radius);
    return aabb(center-size, 2.f*size);
}

vec3f Sphere::color(const vec3f &pt) const {
    if (!matrl.tex.valid())
        return matrl.color;
    const vec3f VN(0,1,0);
    const vec3f VE(1,0,0);
    const vec3f VC = cross(VN,VE);

    vec3f vp = (pt - center)/radius;
    float phi = acos(-dot(vp, VN));
    float the = (acos(dot(VE,vp)/std::sin(phi)))*2.f/M_PI;
    vec2f uv;
    uv.x = matrl.uvscale.x*(dot(VC,vp)>=0.f ? (1.f - the) : (the));
    uv.y = phi/(M_PI*matrl.uvscale.y);
    return matrl.color*matrl.tex.rgb_uvf(uv);
}

vec3f Plane::normal(const vec3f &) const {
    return norm;
}

eSect Plane::intersect(const Ray &ray, float &dist) const {
    last_rayid = ray.id;
    float det = dot(norm, ray.dir);
    //TODO: omg float 0
    if (det != 0.f) {
        float test_dist = -(dot(norm, ray.point) + this->dist)/det;
        if (test_dist > 0) {
            if (test_dist < dist) {
                dist = test_dist;
                return eSect::HIT;
            }
        }
    }
    return eSect::MIS;
}

bool Plane::intersect(const aabb &box) const {
    vec3f v[2] = {box.posi, box.posi + box.size};
    int side[2] = {0,0};
    for (int i = 0; i < 8; ++i) {
        vec3f p(v[i&1].x, v[(i>>1)&1].y, v[(i>>2)&1].z);
        ++side[((dist + dot(p,norm)) < 0.f) ? 0 : 1];
    }
    return (side[0] && side[1]);
    /// v7 code
//    vec3f vmin, vmax;
//    const vec3f& max_box;
//    bvec3 gz = gtv(norm, vec3f(0.f));
//    vec3f k = ifv(gz, vec3f(1.f), vec3f(-1.f));
//    vmax =  k*max_box - posi;
//    vmin = -k*max_box - posi;
//    if (dot(norm, vmin) > 0.0f) return false;
//    if (dot(norm, vmax) >= 0.0f) return true;
//    return false;
}

aabb Plane::get_aabb() const {
    return aabb(vec3f(-10000), vec3f(20000));
}

vec3f Plane::color(const vec3f &pt) const {
    if (!matrl.tex.valid())
        return matrl.color;
    //vec3f uaxis = vec3f(norm.y, norm.z, -norm.x);
    vec3f uaxis = vec3f(norm.y, -norm.x, 0.f);
    vec3f vaxis = cross(uaxis, norm);
    vec2f uv = matrl.uvscale*vec2f(dot(pt,uaxis), dot(pt,vaxis));
    return matrl.color*matrl.tex.rgb_uvf(frac(uv));
}

void Scene::init() {
    //prim.push_back(make_plane(0.f,1.f,0.f, 4.4f, 0.4f,0.3f,0.3f, 1.f, 0.f, 0.f, 0.f,0));
    prim.push_back(make_sphere(2.f,-0.8f,3.f,2.5f,0.7f,0.7f,0.7f,0.3f,0.7f, 0.8f, 1.3f,0, 0.1f));

    {
        //prim.push_back(make_sphere(-5.5f,-0.5f,7.f,2.f,0.7f,0.7f,1.f,0.1f,1.f,0,0,0));
        Primitive* bk = make_sphere(-5.5f,-0.5f,7.f,2.f,0.7f,0.7f,1.f,0.1f,1.f,0,0,0);
        read_tga("textures/marble.tga", bk->matrl.tex);
        prim.push_back(bk);
    }

    {
        //prim.push_back(make_sphere(-1.5f,-3.8f,1.f,1.5f,1.f,0.4f,0.4f,0.2,0,0.8,1.5f,0));
        Primitive* bk = make_sphere(-3.f,3.5f,3.f,1.5f, 0.8f,0.6f,0.3f, 0.9f,0,0,2.f, 0);
        read_tga("textures/wood.tga", bk->matrl.tex);
        prim.push_back(bk);
    }

//    prim.push_back(make_sphere(0,5,5,0.1f,0.6f,0.6f,0.6f,0,0,0,0,1));
//    prim.push_back(make_sphere(-3,5,1,0.1f,1.f,0.7f,0.9f,0,0,0,0,1));

    if (0) {
        Primitive* bk = make_plane(0.4f,0.f,-1.f,12, 0.5f,0.2f,0.7f, 1.f, 0.f,0.f,0.f, 0);
        read_tga("textures/chess.tga", bk->matrl.tex);
        bk->matrl.uvscale = vec2f(0.1f);
        prim.push_back(bk);
    }

    //prim.push_back(make_plane(0,-1,0,7.4,0.4f,0.7f,0.7f,0.5f,0.f,0.f,0.f,0));

    {
        Box* b = new Box(vec3f(2.f,3.8f,1.f), vec3f(2,0.1f,2));
        b->setlight();
        b->matrl.color = vec3f(1.f);
        b->matrl.diff = 0.5f;
        prim.push_back(b);
    }

    {
        Box* b = new Box(vec3f(-5.f,-3.f,0.f), vec3f(2,2,2));
        b->matrl.color = vec3f(1.f);
        b->matrl.diff = 1.f;
        b->matrl.spec = 0.f;
        read_tga("textures/marble.tga", b->matrl.tex);
        prim.push_back(b);

        //prim.push_back(make_sphere(-2.f,-3.f,0.f,0.1f, 1.f,1.f,1.f, 0,0,0,0,1));
    }

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 7; ++y) {
            prim.push_back(make_sphere(
                               -4.5f + x*1.5f, -4.3f + y*1.5f, 10, 0.3f,
                               0.3f, 1.f, 0.4f,
                               0.6f, 0,0,0, 0 ));
        }
    }

    build_grid();
}

Plane *Scene::make_plane(float x, float y, float z, float d,
                         float cr, float cg, float cb,
                         float diff, float refl, float refr, float refi,
                         bool light)
{
    Plane* pl = new Plane(vec3f(x,y,z),d);
    pl->matrl.color = vec3f(cr,cg,cb);
    pl->matrl.diff = diff;
    pl->matrl.refl = refl;
    pl->matrl.refra = refr;
    pl->matrl.refrindx = refi;
    pl->light = light;
    return pl;
}

void Scene::build_grid() {
    vec3f p1(-14,-5,-6); vec3f p2(14,8,30);
    vec3f cellsz = (p2 - p1)/float(gridsize);
    extends = aabb(p1, p2 - p1);
    // store primitives in grid
    for (Primitive* p: prim) {
        // store lights separately
        if (p->light) lights.push_back(p);
        aabb bound = p->get_aabb();
        vec3f bv1 = bound.posi; vec3f bv2 = bv1 + bound.size;

        vec3i v1 = (bv1 - p1)/cellsz;
        vec3i v2 = (bv2 - p1)/cellsz + vec3f(1.f);
        v1 = clamp(v1, 0, gridsize);
        v2 = clamp(v2, 0, gridsize);

        // loop all cells
        for (int x = v1.x; x < v2.x; ++x) {
            for (int y = v1.y; y < v2.y; ++y) {
                for (int z = v1.z; z < v2.z; ++z) {
                    int idx = x + y*gridsize + z*sq(gridsize);
                    vec3f pos = p1 + vec3f(x,y,z)*cellsz;
                    aabb cellbox(pos, cellsz);
                    // intersect primitives for curent cell
                    if (p->intersect(cellbox)) {
                        grid[idx].push_back(p);
                    }
                }
            }
        }
    }
}

Sphere *Scene::make_sphere(float x, float y, float z, float r,
                           float cr, float cg, float cb,
                           float diff, float refl, float refr, float refi,
                           bool light, float drefl)
{
    Sphere* sp = new Sphere(vec3f(x,y,z),r);
    sp->matrl.color = vec3f(cr,cg,cb);
    sp->matrl.diff = diff;
    sp->matrl.refl = refl;
    sp->matrl.refra = refr;
    sp->matrl.refrindx = refi;
    sp->matrl.drefl = drefl;
    sp->light = light;
    return sp;
}

Primitive* Raytracer::raytrace(Ray& ray, vec3f& color_acc, int depth, float ridx, float& dist) {
    if (depth > max_depth)
        return 0;
    // trace primary ray
    dist = BCC_M_FLOAT_MAX;
    vec3f sect_point;
    Primitive* hitprim = NULL;

    /// Find first primitive hit by ray
    eSect sect_res = find_nearest(ray, dist, hitprim);
    // no hit terminate
    if (sect_res == eSect::MIS)
        return NULL;

    if (       !enable_diffuse
            && !enable_reflection
            && !enable_refraction
            && !enable_shadows
            && !enable_specular)
    {
        color_acc = hitprim->matrl.color;
        return hitprim;
    }

    /// Determine color at intersection point
    if (hitprim->light) {
        color_acc = hitprim->matrl.color;
    } else {
        sect_point = ray.point + dist*ray.dir;
        /// trace lighting
        for (Primitive* light: scene.lights) {
            {
                vec3f N = normalize(hitprim->normal(sect_point));
                vec3f L;

                /// compute shadows
                float shade = 1.f;
                if (!enable_shadows) {
                    if (dynamic_cast<Sphere*>(light))
                        L = normalize(dynamic_cast<Sphere*>(light)->center - sect_point);
                    else if (dynamic_cast<Box*>(light))
                        L = normalize(dynamic_cast<Box*>(light)->posi - sect_point);
                } else
                if (dynamic_cast<Sphere*>(light)) { //TODO: omfg
                    L = dynamic_cast<Sphere*>(light)->center - sect_point;
                    float ldist = L.length();
                    L.normalize();
                    Ray light_ray(sect_point + L*BCC_M_FLOAT_EPSILON, L, ++cur_rayid);
                    Primitive* pr = NULL;
                    find_nearest(light_ray, ldist, pr);
                    if (pr != light) shade = 0;

                } else if (dynamic_cast<Box*>(light)) {
                    if (!enable_shadow_sampling) {
                        shade = 0.f;
                        Box* b = dynamic_cast<Box*>(light);
                        L = b->posi + 0.5f*b->size - sect_point;
                        float ldist = L.length();
                        L.normalize();
                        Ray light_ray(sect_point + L*BCC_M_FLOAT_EPSILON, L, ++cur_rayid);
                        Primitive* pr = NULL;
                        find_nearest(light_ray, ldist, pr);
                        if (pr == light) shade = 1.f;

                    } else {
                        shade = 0.f;
                        Box* b = dynamic_cast<Box*>(light);
                        L = normalize(b->posi + 0.5f*b->size - sect_point);

                        if (enable_montecarlo_shadowing) {
                            /// Monte Carlo grid
                            vec2f bdel(b->size.x*0.25f, b->size.z*0.25f);
                            for (int i = 0; i < samples_shadow; ++i) {
                                vec3f lp(b->grid[2*(i&15)] + bdel.x*rnd(),
                                        b->posi.y,
                                        b->grid[1+2*(i&15)] + bdel.y*rnd());
                                vec3f dir = lp - sect_point;
                                float ldist = dir.length();
                                dir.normalize();
                                Ray light_ray(sect_point + dir*BCC_M_FLOAT_EPSILON, dir, ++cur_rayid);
                                Primitive* pr = NULL;
                                //FindOccluder
                                if (find_nearest(light_ray, ldist, pr) != eSect::MIS) {
                                    if (pr == light) shade += 1.f/(float)samples_shadow;
                                }
                            }
                        } else {
                        /// regular grid version
                            for (int x = 0; x < sqrt(samples_shadow); ++x) {
                                for (int y = 0; y < sqrt(samples_shadow); ++y) {
                                    vec3f lp = b->posi + 0.25f*vec3f(x,0.f,y);
                                    vec3f dir = lp - sect_point;
                                    float ldist = dir.length();
                                    dir.normalize();
                                    Ray light_ray(sect_point + dir*BCC_M_FLOAT_EPSILON, dir, ++cur_rayid);
                                    Primitive* pr = NULL;
                                    if (find_nearest(light_ray, ldist, pr) != eSect::MIS) {
                                        if (pr == light) shade += 1.f/(float)samples_shadow;
                                    }
                                }
                            }
                        }
                    }
                }

                /// compute diffuse lighting
                if (enable_diffuse)
                if (shade > 0.f && hitprim->matrl.diff > 0.f) {
                    float lambert = dot(N,L);
                    if (lambert > 0.f) {
                        float k_diff = shade*lambert*hitprim->matrl.diff;
                        color_acc += k_diff*hitprim->color(sect_point)*light->matrl.color;
                    }
                }

                /// compute specular lighting
                if (enable_specular)
                if (shade > 0.f && hitprim->matrl.spec > 0.f) {
                    vec3f R = L - 2.f*dot(L,N)*N;
                    float spec_fract = dot(ray.dir,R);
                    if (spec_fract > 0.f) {
                        /// Schlick BRDF approx
                        //float k_spec = shade*hitprim->matrl.spec
                        //        *spec_fract/(50.f - 50.f*spec_fract + spec_fract);
                        float k_spec = shade*pow(spec_fract, 20)*hitprim->matrl.spec;
                        color_acc += k_spec*light->matrl.color;
                    }
                }
            }
        }

        /// trace reflection
        if (enable_reflection)
        if (hitprim->matrl.refl > 0.f && depth < max_depth) {
            float drefl = hitprim->matrl.drefl;
            if ((drefl > 0.f) && (depth < 2)) {
                // diffuse reflection only for ptimary rays
                vec3f N = hitprim->normal(sect_point);
                vec3f RP = ray.dir - 2.f*dot(ray.dir,N)*N;
                vec3f RN1 = vec3f(RP.z, RP.y, -RP.x);
                vec3f RN2 = cross(RP, RN1);
                float refl = hitprim->matrl.refl/(float)samples_reflection;
                for (int i = 0; i < samples_reflection; ++i) {
                    vec2f offs = drefl*vec2f(rnd()-0.5f, rnd()-0.5f);//TODO: twister
                    vec3f R = RP + RN1*offs.x + RN2*offs.y*drefl;
                    R.normalize();

                    vec3f refl_color(0.f);
                    float refl_dist;
                    Ray refl_ray(sect_point + R*BCC_M_FLOAT_EPSILON, R, ++cur_rayid);
                    raytrace(refl_ray, refl_color, depth+1, ridx, refl_dist);
                    //color_acc += refl*refl_color*hitprim->matrl.color;
                    color_acc += refl*refl_color*hitprim->color(sect_point);
                }
            } else {
                // perfect reflection
                vec3f N = hitprim->normal(sect_point);
                vec3f R = ray.dir - 2.f*dot(ray.dir,N)*N;
                vec3f refl_color(0.f);
                float refl_dist;
                Ray refl_ray(sect_point + R*BCC_M_FLOAT_EPSILON, R, ++cur_rayid);
                raytrace(refl_ray, refl_color, depth+1, ridx, refl_dist);
                color_acc += hitprim->matrl.refl*refl_color*hitprim->color(sect_point);
            }
        }

        /// trace refraction
        if (enable_refraction)
        if (hitprim->matrl.refra > 0.f && depth < max_depth) {
            float hit_ridx = hitprim->matrl.refrindx;
            float n = ridx/hit_ridx;
            vec3f N = hitprim->normal(sect_point)*(sect_res==eSect::INS?-1.f:1.f);
            float cosI = -dot(N, ray.dir);
            float cosT2 = 1.f - sq(n)*(1.f - sq(cosI));
            if (cosT2 > 0.f) {
                vec3f T = (n*ray.dir) + (n*cosI - sqrt(cosT2))*N;
                vec3f refr_color(0.f);
                float refr_dist;
                Ray refr_ray(sect_point + T*BCC_M_FLOAT_EPSILON, T, ++cur_rayid);
                raytrace(refr_ray, refr_color, depth+1, hit_ridx, refr_dist);

                /// apply beers law
                //vec3f absorb = -refr_dist*0.15f*hitprim->matrl.color;
                vec3f absorb = -refr_dist*0.15f*hitprim->color(sect_point);
                vec3f transp = vec3f(exp(absorb.x), exp(absorb.y), exp(absorb.z));
                color_acc += refr_color*transp;
            }
        }
    }
    return hitprim;
}

void Raytracer::init_render(const vec3f& new_eye, const vec3f& new_tar) {
    rndmz();// for twister

    curline = 20;
    cur_rayid = 0;

//    vec3f new_eye(2,0,5);
//    vec3f new_tar(0,0,0);

    mat4f view = lookat(new_eye, new_tar, vec3f(0,1,0));

    m_eye = vec3f(0,0,-5);
    m_top_left = vec3f(-4,3,0);//TODO: use botom_left luke
    vec3f top_rigt = vec3f(4,3,0);
    vec3f bot_left = vec3f(-4,-3,0);

    m_eye = m2v(view*v2m(m_eye));
    m_top_left = m2v(view*v2m(m_top_left));
    top_rigt = m2v(view*v2m(top_rigt));
    bot_left = m2v(view*v2m(bot_left));

    m_deltax = (top_rigt - m_top_left)/(float)img.width;
    m_deltay = (bot_left - m_top_left)/(float)img.height;

    cell_size = scene.extends.size/(float)scene.gridsize;

    enable_diffuse = true;
    enable_supersampling = true;
    enable_refraction = true;
    enable_reflection = true;
    enable_specular = true;
    enable_shadows = true;
    enable_shadow_sampling = true;
}

bool Raytracer::render() {
    //    Timer timer;
    //    timer.start();
    Primitive* last_horiz_prim = NULL;
    std::vector<Primitive*> last_vertl_prim(img.width, NULL);
    /// For each pixel
    for (uint y = curline; y < (img.height-20); ++y) {
        for (uint x = 0; x < img.width; ++x) {
            vec3f res_color;
            vec3f cur_point = m_top_left + x*m_deltax + y*m_deltay;

            /// Construct ray from camera through pixel
            vec3f acc_color;
            Primitive* prim = render_ray(cur_point, acc_color);

            if (!enable_supersampling) res_color = acc_color;
            else
            /// supersampling
            if (prim != last_horiz_prim || prim != last_vertl_prim[x]) {
                last_horiz_prim = prim;
                last_vertl_prim[x] = prim;
                /// 2x2 supersampling
//                render_ray(cur_point - 0.5f*deltax, acc_color);
//                render_ray(cur_point - 0.5f*deltay, acc_color);
//                render_ray(cur_point - 0.5f*(deltax + deltay), acc_color);
//                res_color = acc_color/4.f;
                /// 3x3 supersampling
                vec2f sst;
                for (sst.y = -1; sst.y < 2; ++sst.y) {
                    for (sst.x = -1; sst.x < 2; ++sst.x) {
                        if (sst.y == 0 && sst.x == 0) continue;// skip central poin already rendered
                        render_ray(cur_point - 0.5f*(m_deltax*sst.x + m_deltay*sst.y), acc_color);
                    }
                }
                res_color = acc_color/9.f;

            } else {
                res_color = acc_color;
            }

            /// Draw color
            img.pix(x,y) = rgb2i(clamp_color(255.f*res_color));
        }

        //        if (timer.time() > 1000) {
        //            curline = y + 1;
        //            return false;
        //        }
    }
    return true;
}

eSect Raytracer::find_nearest(const Ray &ray, float& dist, Primitive *&prim) {
    return find_nearest_grid(ray, dist, prim);
}

eSect Raytracer::find_nearest_forall(const Ray &ray, float& dist, Primitive *&prim) {
    eSect sect_res = eSect::MIS;
    for (Primitive* pr: scene.prim) {
        eSect esect = pr->intersect(ray, dist);
        if (esect != eSect::MIS) {
            prim = pr;
            sect_res = esect;
        }
    }
    return sect_res;
}

eSect Raytracer::find_nearest_grid(const Ray &ray, float& dist, Primitive *&prim) {
    eSect retval = eSect::MIS;
    vec3f curpos = ray.point;
    vec3f raydir = ray.dir;
    aabb e = scene.extends;
    int gridsize = scene.gridsize;
    // setup 3DDDA - double check reusability of primary ray
    vec3f cell = (curpos - e.posi)/cell_size;
    vec3i P(cell);

    if ( !(gt(P, vec3i(0)) && gt(vec3i(gridsize-1), P)) )
        return eSect::MIS;

    bvec3 gr = gtv(raydir, vec3f(0.f));
    vec3i step = ifv(gr, vec3i(1), vec3i(-1));
    vec3i out = ifv(gr, vec3i(gridsize), vec3i(-1));
    vec3f cb = e.posi + ( ifv(gr, vec3f(1.f), vec3f(0.f)) + vec3f(P) )*cell_size;

    //bvec3 ne = neqv(raydir, vec3f(0.f));
    bvec3 ne = not(eqv(raydir, vec3f(0.f)));
    vec3f tmax = ifv(ne, (cb - curpos)/raydir, vec3f(1000000));
    vec3f tdel = ifv(ne, cell_size*vec3f(step)/raydir, vec3f(0.f));//TODO: else not given

    // start steping: trace primary ray
    prim = NULL;
    while (1) {
        bool break2 = false;
        auto list = scene.grid[P.x + (P.y * gridsize) + (P.z * sq(gridsize))];
        for (Primitive* pr: list) {
            if (pr->last_rayid != ray.id) {
                eSect result = pr->intersect(ray, dist);
                if (result != eSect::MIS) {
                    retval = result;
                    prim = pr;
                    break2 = true;
                    break;
                    //goto testloop;
                }
            }
        }

        if (break2) break;

        int minxyz = min3i(tmax.x, tmax.y, tmax.z);
        P[minxyz] += step[minxyz];
        if (P[minxyz] == out[minxyz])
            return eSect::MIS;
        tmax[minxyz] += tdel[minxyz];
    }

    // testloop
    while (1) {
        auto list = scene.grid[P.x + (P.y * gridsize) + (P.z * sq(gridsize))];
        for(Primitive* pr: list) {
            if (pr->last_rayid != ray.id) {
                eSect result = pr->intersect(ray, dist);
                if (result != eSect::MIS)  {
                    retval = result;
                    prim = pr;
                }
            }
        }

        int minxyz = min3i(tmax.x, tmax.y, tmax.z);
        if (dist < tmax[minxyz]) break;
        P[minxyz] += step[minxyz];
        if (P[minxyz] == out[minxyz]) break;
        tmax[minxyz] += tdel[minxyz];
    }

    return retval;
}

Primitive *Raytracer::render_ray(const vec3f &screen_pos, vec3f &acc) {
    aabb e = scene.extends;
    vec3f dir = normalize(screen_pos - m_eye);
    Ray ray(m_eye, dir, ++cur_rayid);
    // advance ray to scene bounding box
    if (!e.contains(m_eye)) {
        float bdist = 10000.f;
        Box b(e.posi, e.size);
        if (b.intersect(ray, bdist) != eSect::MIS) {
            ray.point = m_eye + (bdist + BCC_M_FLOAT_EPSILON)*dir;
        }
    }
    float dist;
    return raytrace(ray, acc, 1, 1.f, dist);
}

eSect Box::intersect(const Ray &ray, float &dist) const {
//    last_rayid = ray.id;
//    vec3f rpos = ray.point;
//    vec3f invd = 1.f/ray.dir;
//    vec3f bmin = posi; vec3f bmax = bmin + size;

//    float lo,hi,tmin,tmax;

//    lo = invd.x * (bmin.x - rpos.x);
//    hi = invd.x * (bmax.x - rpos.x);
//    tmin = std::min(lo,hi);
//    tmax = std::max(lo,hi);

//    lo = invd.y * (bmin.y - rpos.y);
//    hi = invd.y * (bmax.y - rpos.y);
//    tmin = std::max(tmin, std::min(lo,hi));
//    tmax = std::min(tmax, std::max(lo,hi));

//    lo = invd.z * (bmin.z - rpos.z);
//    hi = invd.z * (bmax.z - rpos.z);
//    tmin = std::max(tmin, std::min(lo,hi));
//    tmax = std::min(tmax, std::max(lo,hi));

//    dist = tmin;

//    if (tmin <= tmax && tmax > 0.f) return eSect::HIT;
//    else return eSect::MIS;

    last_rayid = ray.id;
    eSect retval = eSect::MIS;
    vec3f d = ray.dir; vec3f o = ray.point;
    vec3f v1 = posi; vec3f v2 = v1 + size;
    vec3f distv[2];
    //bvec3 nz = neqv(d, vec3f(0.f));
    bvec3 nz = not(eqv(d, vec3f(0.f)));
    distv[0] = ifv(nz, (v1 - o)/d, vec3f(-1.f));
    distv[1] = ifv(nz, (v2 - o)/d, vec3f(-1.f));
    const vec3f eps(BCC_M_FLOAT_EPSILON);
    for (int i = 0; i < 6; i++ ) {
        float fd = distv[i%2][i/2];
        if (fd > 0) {
            vec3f ip = o + fd*d;
            if (gt(ip, v1-eps) && gt(v2+eps, ip) && fd < dist) {
                dist = fd;
                retval = eSect::HIT;
            }
        }
    }
    return retval;
}

bool Box::intersect(const aabb &box) const {
    vec3f v1 = box.posi; vec3f v2 = box.posi + box.size;
    vec3f v3 = posi; vec3f v4 = posi + size;
    return gt(v4,v1) && gt(v2,v3);
}

aabb Box::get_aabb() const {
    return aabb(posi,size);
}

vec3f Box::normal(const vec3f& /*pt*/) const {
    // shpere simplification
    //return pt - (posi+size)/2.f;

//    vec3f tpc = pt - 0.5f*(posi + size);
//    vec3f atpc = abs(tpc);
//    int i = max3i(atpc.x, atpc.y, atpc.z);
//    vec3f res(0.f);
//    res[i] = (tpc[i]>0 ? 1.f : -1.f);
//    return res;


    vec3f dist[2];
    dist[0] = abs(size - posi);
    dist[1] = abs(2.f*size - posi);
    int best = 0;
    float bdist = dist[0][0];
    for (int i = 1; i < 6; i++) {
        if (dist[i%2][i/2] < bdist) {
            bdist = dist[i%2][i/2];
            best = i;
        }
    }
    vec3f res(0.f);
    res[best/2] = 2.f*(best%2) - 1.f;
    return res;
}

void Box::setlight() {
    light = true;
    grid = {1,2,3,3,2,0,0,1,2,3,0,3,0,0,2,2,3,1,1,3,1,0,3,2,2,1,3,0,1,1,0,2};
    for (int i = 0; i < 16; ++i) {
        grid[2*i] = grid[2*i]*size.x/4.f + posi.x;
        grid[2*i+1] = grid[2*i+1]*size.z/4.f + posi.z;
    }
}

vec3f Box::color(const vec3f& pt) const {
    //TODO: find plane of pt, uv = (pt - corner_point)/plane_size
    if (!matrl.tex.valid())
        return matrl.color;

    vec3f n = normal(pt);
    vec2f uv;
    for (int i = 0; i < 3; ++i) {
        if (n[i] != 0.f) {
            int i1 = (i+1)%3;
            int i2 = (i+2)%3;
            uv.x = matrl.uvscale.x*(pt[i1] - posi[i1])/size[i1];
            uv.y = matrl.uvscale.y*(pt[i2] - posi[i2])/size[i2];
            break;
        }
    }
    return matrl.color*matrl.tex.rgb_uvf(uv);
}

vec3f Triangle::normal(const vec3f &/*pt*/) const {
    return cross(ver[1] - ver[0], ver[2] - ver[0]);
}

/*  |t|             | T E1 E2|
 *  |u| = ___1____* |-D  T E2|
 *  |v|  |-D E1 E2| |-D E1  T|
 *
 *  T = O - v0; E1 = v1 - v0; E2 = v2 - v0
 */
eSect Triangle::intersect(const Ray &ray, float &dist) const {
    last_rayid = ray.id;
    vec3f e1 = ver[1] - ver[0];
    vec3f e2 = ver[2] - ver[0];
    vec3f de2 = cross(ray.dir, e2);
    float det = dot(de2, e1);
    if (eq(det,0.f)) return eSect::MIS;
    float inv_det = 1.f/det;

    vec3f t = ray.point - ver[0];
    float u = inv_det*dot(t, de2);
    if (u < 0.f || u > 1.f) return eSect::MIS;

    vec3f te1 = cross(t, e1);
    float v = inv_det*dot(te1, ray.dir);
    if (v < 0.f || u + v > 1.f) return eSect::MIS;

    float tdist = inv_det*dot(e2, te1);
    if (tdist < dist) {
        dist = tdist;
        return eSect::HIT;
    }
    return eSect::MIS;
}

bool Triangle::intersect(const aabb &/*box*/) const {
    return false;
}

aabb Triangle::get_aabb() const {
    aabb box;
    box.posi = min(min(ver[0],ver[1]),ver[2]);
    box.size = max(max(ver[0],ver[1]),ver[2]) - box.posi;
    return box;
}

vec3f Triangle::color(const vec3f &/*pt*/) const {
    return matrl.color;
}

} // namespace bacchus
