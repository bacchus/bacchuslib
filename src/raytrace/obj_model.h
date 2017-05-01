#pragma once

#include <vector>
#include <string>

#include "math/vec3.h"
#include "math/vec2.h"

namespace bacchus {

class Model {
public:
    Model(const char *filename);
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

    vec3f m_center;
    vec3f m_size;
    float m_radius;
};

} // namespace bacchus
