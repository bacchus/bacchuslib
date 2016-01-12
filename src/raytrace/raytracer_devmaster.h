#pragma once

#include <list>

#include "image/image.h"
#include "math/vec3.h"
#include "math/mdefs.h"

namespace bacchus {

enum class eSect { HIT, MIS, INS };

class aabb {
public:
    vec3f posi;
    vec3f size;

    aabb()
        : posi(0.f), size(0.f)
    {}

    aabb(const vec3f& pos, const vec3f& sz)
        : posi(pos), size(sz)
    {}

    bool contains(const vec3f& pt) const {
        vec3f v1 = posi; vec3f v2 = posi + size;
        return gt(pt,v1) && gt(v2,pt);
    }

    bool intersect(const aabb& ab2) const {
        vec3f v1 = ab2.posi; vec3f v2 = ab2.posi + ab2.size;
        vec3f v3 = posi; vec3f v4 = posi + size;
        return gt(v4,v1) && gt(v2,v3);
    }
};

class Material {
public:
    vec3f color;
    float diff;
    float spec;
    float refl;
    float drefl;
    float refra;
    float refrindx;

    vec2f uvscale;
    Image tex;

    Material()
        : color(0.2f)
        , diff(0.2f), spec(0.8f)
        , refl(0.f), drefl(0)
        , refra(0.f), refrindx(1.5f)
        , uvscale(1.f)
    {}
};

class Ray {
public:
    vec3f point;
    vec3f dir;
    int id;

    Ray(vec3f point, vec3f dir, int id)
        : point(point), dir(dir), id(id)
    {}
};

class Primitive {
public:
    Material matrl;
    bool light;
    Primitive(): light(false), last_rayid(-1) {}
    virtual ~Primitive() {}
    virtual vec3f normal(const vec3f& pt) const = 0;
    virtual eSect intersect(const Ray& ray, float& dist) const = 0;

    mutable int last_rayid;
    //TODO: check arch approach - move logic to aabb
    //aabb* m_aabb;
    virtual bool intersect(const aabb& box) const = 0;
    virtual aabb get_aabb() const = 0;
    // tex
    virtual vec3f color(const vec3f& pt) const = 0;
};

class Triangle: public Primitive {
public:
    vec3f ver[3];

    Triangle(const vec3f& v0, const vec3f& v1, const vec3f& v2)
        : ver{v0,v1,v2}
    {}
    virtual vec3f normal(const vec3f&) const override;
    virtual eSect intersect(const Ray &ray, float& dist) const override;
    // aabb
    virtual bool intersect(const aabb&) const override;
    virtual aabb get_aabb() const override;
    // tex
    virtual vec3f color(const vec3f &) const override;
};

class Sphere: public Primitive {
public:
    vec3f center;
    float radius;
    Sphere(const vec3f& center, float radius)
        : center(center), radius(radius)
    {}
    virtual vec3f normal(const vec3f& pt) const override;
    virtual eSect intersect(const Ray &ray, float& dist) const override;
    // aabb
    virtual bool intersect(const aabb& box) const override;
    virtual aabb get_aabb() const override;
    // tex
    virtual vec3f color(const vec3f &pt) const override;
};

class Plane: public Primitive {
public:
    vec3f norm;
    float dist;
    Plane(vec3f norm, float dist)
        : norm(norm), dist(dist)
    {}
    virtual vec3f normal(const vec3f& /*pt*/) const override;
    virtual eSect intersect(const Ray& ray, float& dist) const override;

    virtual bool intersect(const aabb& box) const override;
    virtual aabb get_aabb() const override;
    // tex
    virtual vec3f color(const vec3f &pt) const override;
};

class Box: public Primitive {
public:
    vec3f posi;
    vec3f size;
    std::vector<float> grid;
    void setlight();

    Box(const vec3f& pos, const vec3f& siz)
        : posi(pos), size(siz)
    {}
    virtual vec3f normal(const vec3f&) const override;
    virtual eSect intersect(const Ray& ray, float& dist) const override;

    virtual bool intersect(const aabb& box) const override;
    virtual aabb get_aabb() const override;
    // tex
    virtual vec3f color(const vec3f &pt) const override;
};

class Scene {
public:
    std::vector<Primitive*> prim;
    std::vector<Primitive*> lights;
    Scene() { grid.resize(gridsize*gridsize*gridsize); }
    ~Scene() { for (auto& p: prim) if (p) delete p; }
    void init();

    Sphere* make_sphere(float x, float y, float z, float r,
                        float cr, float cg, float cb,
                        float diff, float refl, float refr, float refi,
                        bool light, float drefl=0);
    Plane *make_plane(float x, float y, float z, float d,
                      float cr, float cg, float cb,
                      float diff, float refl, float refr, float refi,
                      bool light);

    aabb extends;
    int gridsize = 8;
    std::vector< std::vector<Primitive*> > grid;
    void build_grid();
};

class Raytracer {
public:
    Image& img;
    Scene scene;

    vec3f m_eye,m_top_left,m_deltax,m_deltay;
    int curline;

    Raytracer(Image& img): img(img) { }
    Primitive* raytrace(Ray& ray, vec3f& acc, int depth, float ridx, float& dist);
    void init_render(const vec3f &new_eye, const vec3f &new_tar);
    bool render();

    vec3f cell_size;
    int cur_rayid;
    eSect find_nearest_forall(const Ray &ray, float &dist, Primitive*& prim);
    eSect find_nearest_grid(const Ray &ray, float &dist, Primitive*& prim);
    eSect find_nearest(const Ray &ray, float &dist, Primitive*& prim);
    Primitive* render_ray(const vec3f& screen_pos, vec3f& acc);

    // settings
    int max_depth = 4;
    int samples_super = 10;
    int samples_shadow = 10;
    int samples_reflection = 10;

    bool enable_supersampling = false;
    bool enable_refraction = false;
    bool enable_reflection = false;
    bool enable_specular = false;
    bool enable_shadows = false;
    bool enable_diffuse = false;
    bool enable_shadow_sampling = false;
    bool enable_montecarlo_shadowing = false;
};

} // namespace bacchus
