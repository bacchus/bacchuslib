#pragma once

#include <map>
#include <vector>
#include <string>

#include "math/vec4.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace bacchus {

class Model {
public:
    struct Material {

        Material()
            : ambi(0.2f, 0.2f, 0.2f, 1.0f)
            , diff(0.8f, 0.8f, 0.8f, 1.0f)
            , spec(0.0f, 0.0f, 0.0f, 1.0f)
            , shine(0.0f), alpha(1.0f)
        {}

        vec4f ambi;
        vec4f diff;
        vec4f spec;
        float shine;
        float alpha;
        std::string name;
        std::string colorMap;
        std::string bumpMap;
    };

    Model(const std::string& filename);
    ~Model();

    int nverts() const;
    int nfaces() const;
    std::vector<int> face(int idx) const;
    vec3f normal(int iface, int nthvert) const;
    vec3f vert(int i) const;
    vec3f vert(int iface, int nthvert) const;
    vec2f uv(int iface, int nthvert) const;
    //TGAColor diffuse(vec2f uv);
    //float specular(vec2f uv);
    //vec3f normal(vec2f uv);

private:
    void loadMtllib(const std::string& filename);
    void calcBounds();

private:
    std::vector< std::vector<vec3i> > m_faces; // vertex/uv/normal
    std::vector<vec3f> m_verts;
    std::vector<vec3f> m_norms;
    std::vector<vec2f> m_uv;
    //TGAImage diffusemap_;
    //TGAImage normalmap_;
    //TGAImage specularmap_;
    //void load_texture(std::string filename, const char *suffix, TGAImage &img);

    std::vector<Material> m_mtl;
    std::map<std::string, int> m_mtl_cache;

    vec3f m_center;
    vec3f m_size;
    float m_radius;
};

} // namespace bacchus
