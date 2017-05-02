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

    struct Vertex {

        Vertex(const vec3f& vpos
               , const vec2f& vtex
               , const vec3f& vnrm)
            : pos(vpos), tex(vtex), nrm(vnrm)
        {}

        vec3f pos;
        vec2f tex;
        vec3f nrm;
    };

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
    vec3f vert(int i) const;
    vec3f vert(int iface, int nthvert) const;
    vec2f uv(int iface, int nthvert) const;
    vec3f normal(int iface, int nthvert) const;
    //TGAColor diffuse(vec2f uv);
    //float specular(vec2f uv);
    //vec3f normal(vec2f uv);

    //TODO: this is material quality
    bool transparent() const { return false; }
    // TODO: here faces must be triangles
    int vertex_count() const { return m_mesh.size(); }
    const float* pos_ptr() const { return &m_mesh[0].pos.x; }
    const float* tex_ptr() const { return &m_mesh[0].tex.x; }
    const float* nrm_ptr() const { return &m_mesh[0].nrm.x; }

    vec3f center() const { return m_center; }
    vec3f size() const { return m_size; }
    float radius() const { return m_radius; }

private:
    void loadMtllib(const std::string& filename);
    void calcBounds();
    void buildMeshes();

private:
    std::vector< std::vector<vec3i> > m_faces; // vertex/uv/normal
    std::vector<vec3f> m_verts;
    std::vector<vec2f> m_uv;
    std::vector<vec3f> m_norms;
    //TGAImage diffusemap;
    //TGAImage normalmap;
    //TGAImage specularmap;
    //void load_texture(std::string filename, const char *suffix, TGAImage &img);

    std::vector<Vertex> m_mesh;

    std::vector<Material> m_mtl;
    std::map<std::string, int> m_mtl_cache;

    vec3f m_center;
    vec3f m_size;
    float m_radius;
};

} // namespace bacchus
