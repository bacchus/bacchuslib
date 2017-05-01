#include "obj_model.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace bacchus {

Model::Model(const std::string& filename)
    : m_faces()
    , m_verts()
    , m_norms()
    , m_uv()
    //, diffusemap_(), normalmap_(), specularmap_()
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    int cur_mtl = 0;

    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;

        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            m_verts.push_back(v);

        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vec3f n;
            for (int i=0;i<3;i++) iss >> n[i];
            m_norms.push_back(n);

        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            vec2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            m_uv.push_back(uv);

        }  else if (!line.compare(0, 2, "f ")) {
            // v, v//vn, v/vt, v/vt/vn
            // in wavefront obj all indices start at 1, not zero
            // so we use: tmp -= vec3i(1);
            std::vector<vec3i> f;
            vec3i tmp;
            iss >> trash;

//            if (line.find("//") != std::string::npos) {
//                // v//vn case 2: must cope with it
//                while (iss >> tmp[0] >> trash >> trash >> tmp[2])
//                    f.push_back(tmp - vec3i(1));
//                m_faces.push_back(f);
//            } else
            {
                std::string token;
                iss >> token;
                std::istringstream token_iss(token.c_str());

                int n = 0;
                std::string::size_type pos = token.find_first_of("/", 0);
                while (pos != std::string::npos) {
                    pos = token.find_first_of("/", pos);
                    ++n;
                }

                switch (n) {
                case 0:
                    token_iss >> tmp[0];
                    f.push_back(tmp - vec3i(1));
                    while (iss >> tmp[0])
                        f.push_back(tmp - vec3i(1));
                    break;

                case 1:
                    token_iss >> tmp[0] >> trash >> tmp[1];
                    f.push_back(tmp - vec3i(1));
                    while (iss >> tmp[0] >> trash >> tmp[1])
                        f.push_back(tmp - vec3i(1));
                    break;

                case 2:
                    token_iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2];
                    f.push_back(tmp - vec3i(1));
                    while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
                        f.push_back(tmp - vec3i(1));
                    break;

                default:
                    break;
                }
            }

            m_faces.push_back(f);

            // BCC: added 'v, v//vn, v/vt, v/vt/vn' variations
//            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
//                // in wavefront obj all indices start at 1, not zero
//                for (int i=0; i<3; i++) tmp[i]--;
//                f.push_back(tmp);
//            }
//            m_faces.push_back(f);

        } else if (!line.compare(0, 7, "mtllib ")) {
            // mtllib
            size_t pos = filename.find_last_of("\\/");
            std::string name = (pos == std::string::npos
                                ? "" : filename.substr(0, pos));
            name += line.substr(7);
            loadMtllib(name);

        } else if (!line.compare(0, 7, "usemtl ")) {
            // usemtl
            std::string name = line.substr(7);
            cur_mtl = (m_mtl_cache.find(name) == m_mtl_cache.end()
                       ? 0 : m_mtl_cache[name]);
        }
    }
    //std::cout << "# v# " << m_verts.size() << " f# "  << m_faces.size() << " vt# " << m_uv.size() << " vn# " << m_norms.size() << std::endl;
    //load_texture(filename, "_diffuse.tga", diffusemap_);
    //load_texture(filename, "_nm_tangent.tga",      normalmap_);
    //load_texture(filename, "_spec.tga",    specularmap_);

    //    buildMeches();
    //    calcBounds();
    //    genNorms();
    //    genTangs();
}

Model::~Model() {
    m_faces.clear();
    m_verts.clear();
    m_norms.clear();
    m_uv.clear();
}

int Model::nverts() const {
    return (int)m_verts.size();
}

int Model::nfaces() const {
    return (int)m_faces.size();
}

std::vector<int> Model::face(int idx) const {
    std::vector<int> face;
    for (int i=0; i<(int)m_faces[idx].size(); i++)
        face.push_back(m_faces[idx][i][0]);
    return face;
}

vec3f Model::vert(int i) const {
    return m_verts[i];
}

vec3f Model::vert(int iface, int nthvert) const {
    return m_verts[m_faces[iface][nthvert][0]];
}

vec3f Model::normal(int iface, int nthvert) const {
    int idx = m_faces[iface][nthvert][2];
    return normalize(m_norms[idx]);
}

vec2f Model::uv(int iface, int nthvert) const {
    return m_uv[m_faces[iface][nthvert][1]];
}

void Model::loadMtllib(const std::string &filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    int cur_mtl = 0;
    int illum = 0;

    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        std::string trash;
        float ftmp;
        vec3f tmp;

        if (!line.compare(0, 7, "newmtl ")) {
            Material mtl;
            cur_mtl = m_mtl.size();
            mtl.name = line.substr(7);
            m_mtl_cache[mtl.name] = cur_mtl;
            m_mtl.push_back(mtl);

        } else if (!line.compare(0, 3, "Ns ")) {
            // shininess is 0..1000 scale to 0..1
            iss >> trash;
            iss >> ftmp;
            m_mtl[cur_mtl].shine = ftmp/1000.0f;

        } else if (!line.compare(0, 3, "Tr ")) {
            iss >> trash;
            iss >> ftmp;
            m_mtl[cur_mtl].alpha = 1.0f - ftmp;

        } else if (!line.compare(0, 1, "d")) {
            iss >> trash;
            iss >> ftmp;
            m_mtl[cur_mtl].alpha = ftmp;

        } else if (!line.compare(0, 3, "Ka ")) {
            iss >> trash;
            iss >> tmp[0] >> tmp[1] >> tmp[2];
            m_mtl[cur_mtl].ambi = vec4f(ftmp, 1.0f);

        } else if (!line.compare(0, 3, "Kd ")) {
            iss >> trash;
            iss >> tmp[0] >> tmp[1] >> tmp[2];
            m_mtl[cur_mtl].diff = vec4f(ftmp, 1.0f);

        } else if (!line.compare(0, 3, "Ks ")) {
            iss >> trash;
            iss >> tmp[0] >> tmp[1] >> tmp[2];
            m_mtl[cur_mtl].spec = vec4f(ftmp, 1.0f);

        } else if (!line.compare(0, 6, "illum ")) {
            iss >> trash;
            iss >> illum;
            if (illum == 1)
                m_mtl[cur_mtl].spec = vec4f(vec3f(0.0f), 1.0f);

        } else if (!line.compare(0, 7, "map_Kd ")) {
            m_mtl[cur_mtl].colorMap = line.substr(7);

        } else if (!line.compare(0, 9, "map_bump ")) {
            m_mtl[cur_mtl].bumpMap = line.substr(9);

        }
    }
}

void Model::calcBounds()
{
    if (m_verts.empty())
        return;

    vec3f vmin = m_verts[0];
    vec3f vmax = vmin;

    for (int i = 1; i < m_verts.size(); ++i) {
        vec3f v = m_verts[i];
        vmin = min(vmin, v);
        vmax = max(vmax, v);
    }

    m_center = 0.5f*(vmin + vmax);
    m_size = vmax - vmin;
    m_radius = max(m_size);
}

//void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
//    std::string texfile(filename);
//    size_t dot = texfile.find_last_of(".");
//    if (dot!=std::string::npos) {
//        texfile = texfile.substr(0,dot) + std::string(suffix);
//        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
//        img.flip_vertically();
//    }
//}

//TGAColor Model::diffuse(vec2f uvf) {
//    vec2i uv(uvf[0]*diffusemap_.get_width(), uvf[1]*diffusemap_.get_height());
//    return diffusemap_.get(uv[0], uv[1]);
//}

//vec3f Model::normal(vec2f uv) {
//    vec2i uv(uv[0]*normalmap_.get_width(), uv[1]*normalmap_.get_height());
//    TGAColor c = normalmap_.get(uv[0], uv[1]);
//    vec3f res;
//    for (int i=0; i<3; i++)
//        res[2-i] = (float)c[i]/255.f*2.f - 1.f;
//    return res;
//}

//float Model::specular(vec2f uvf) {
//    vec2i uv(uvf[0]*specularmap_.get_width(), uvf[1]*specularmap_.get_height());
//    return specularmap_.get(uv[0], uv[1])[0]/1.f;
//}

} // namespace bacchus
