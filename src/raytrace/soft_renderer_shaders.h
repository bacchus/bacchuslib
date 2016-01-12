#pragma once

#include "soft_renderer.h"

namespace bacchus {

class SimpleShader: public IShader {
    const Model& m_model;
    mat4f m_mvp;
    const Image& m_tex;
    vec2f var_uv[3];
public:
    SimpleShader(const Model& model, const mat4f& mvp, const Image& tex)
        : m_model(model)
        , m_mvp(mvp)
        , m_tex(tex)
    {}

    virtual vec4f vertex(int iface, int nthvert) {
        vec4f gl_vertex = m_mvp*v2m(m_model.vert(iface, nthvert));
        var_uv[nthvert] = vec2f(m_tex.width, m_tex.height)*m_model.uv(iface,nthvert);
        return gl_vertex;
    }

    virtual bool fragment(const vec3f& bar, vec4i& color) {
        vec2i uv = var_uv[0]*bar.x + var_uv[1]*bar.y + var_uv[2]*bar.z;
        color = m_tex.rgba(uv);
        return false;
    }
};

class ShadowShader: public IShader {
    const Model& m_model;
    mat4f m_mvp;
    const Image& m_tex;
    vec2f var_uv[3];
    mat4f m_shadow_mvp;
    const int* m_shadowbuffer;
    mat3f var_tri;
public:
    ShadowShader(const Model& model, const mat4f& mvp, const Image& tex, const mat4f& shadow_mvp, const int* shad)
        : m_model(model)
        , m_mvp(mvp)
        , m_tex(tex)
        , m_shadow_mvp(shadow_mvp)
        , m_shadowbuffer(shad)
    {}

    virtual vec4f vertex(int iface, int nthvert) {
        vec4f gl_vertex = m_mvp*v2m(m_model.vert(iface, nthvert));
        var_uv[nthvert] = vec2f(m_tex.width, m_tex.height)*m_model.uv(iface,nthvert);
        var_tri.set_col(nthvert, m2v(gl_vertex/gl_vertex[3]));
        return gl_vertex;
    }

    virtual bool fragment(const vec3f& bar, vec4i& color) {
        vec3f shp = m2v(m_shadow_mvp*v2m(var_tri*bar));
        int shidx = int(shp.x) + int(shp.y)*m_tex.width;
        float shadow = 0.3f + 0.7f*(m_shadowbuffer[shidx] < shp.z);

        vec2i uv = var_uv[0]*bar.x + var_uv[1]*bar.y + var_uv[2]*bar.z;
        color = vec4i(shadow*m_tex.rgb(uv), 255);
        return false;
    }
};

class DepthShader: public IShader {
    const Model& m_model;
    mat4f m_mvp;
    mat3f var_tri;
public:
    DepthShader(const Model& model, const mat4f& mvp)
        : m_model(model)
        , m_mvp(mvp)
    {}

    virtual vec4f vertex(int iface, int nthvert) {
        vec4f gl_vertex = m_mvp*v2m(m_model.vert(iface, nthvert));
        var_tri.set_col(nthvert, m2v(gl_vertex/gl_vertex[3]));
        return gl_vertex;
    }

    virtual bool fragment(const vec3f& bar, vec4i& color) {
        vec3f p = var_tri*bar;
        color = vec4i(vec3i(p.z), 255);
        return false;
    }
};

class GouraudShader: public IShader {
    const Model& m_model;
    mat4f m_mvp;
    vec3f m_ldir;
    const Image& m_tex;
    vec3f var_intens;
    vec2f var_uv[3];
public:
    GouraudShader(const Model& model, const mat4f& mvp, const vec3f& ldir, const Image& tex)
        : m_model(model)
        , m_mvp(mvp)
        , m_ldir(ldir)
        , m_tex(tex)
    {}

    virtual vec4f vertex(int iface, int nthvert) {
        vec4f gl_vertex = v2m(m_model.vert(iface, nthvert));
        var_uv[nthvert] = vec2f(m_tex.width, m_tex.height)*m_model.uv(iface,nthvert);
        return m_mvp*gl_vertex;
    }

    virtual bool fragment(const vec3f& bar, vec4i& color) {
        vec2i uv = var_uv[0]*bar.x + var_uv[1]*bar.y + var_uv[2]*bar.z;
        float intens = dot(var_intens, bar);
        if      (intens>0.85) intens = 1.f;
        else if (intens>0.60) intens = 0.8f;
        else if (intens>0.45) intens = 0.6f;
        else if (intens>0.30) intens = 0.4f;
        else if (intens>0.15) intens = 0.3f;
        else                  intens = 0.f;
        color = vec4f(intens*m_tex.rgb(uv), 255);
        //color = vec4f(intens*vec3f(250, 180, 50), 255);
        return false;
    }
};

class NormMapingShader: public IShader {
    const Model& m_model;
    mat4f m_mvp;
    mat4f m_mvp_invtr;
    vec3f m_ldir;
    vec3f m_edir;
    const Image& m_tex;
    const Image& m_norm;
    const Image& m_spec;
    vec2f var_uv[3];
    // shadow
    mat4f m_shadow_mvp;
    const int* m_shadowbuffer;
    mat3f var_tri;
    vec3f var_pos;
public:
    NormMapingShader(const Model& model, const mat4f& mvp, const vec3f& ldir, const vec3f& edir
                     , const Image& tex, const Image& norm, const Image& spec
                     , const mat4f& shadow_mvp, const int* shad)// shadow
        : m_model(model)
        , m_mvp(mvp)
        , m_mvp_invtr(mvp.inverse().transpose())
        , m_ldir(ldir)
        , m_edir(edir)
        , m_tex(tex)
        , m_norm(norm)
        , m_spec(spec)
        // shadow
        , m_shadow_mvp(shadow_mvp)
        , m_shadowbuffer(shad)
    {}

    virtual vec4f vertex(int iface, int nthvert) {
        vec4f gl_vertex = m_mvp*v2m(m_model.vert(iface, nthvert));
        var_uv[nthvert] = vec2f(m_tex.width, m_tex.height)*m_model.uv(iface,nthvert);
        // shadow
        //var_tri.set_col(nthvert, m2v(gl_vertex/gl_vertex[3]));
        var_pos = m2v(gl_vertex);
        var_tri.set_col(nthvert, var_pos);
        return gl_vertex;
    }

    virtual bool fragment(const vec3f& bar, vec4i& color) {
        // shadow
        vec3f shp = m2v(m_shadow_mvp*v2m(var_tri*bar));
        int shidx = int(shp.x) + int(shp.y)*m_tex.width;
        float shadow = 0.3f + 0.7f*(m_shadowbuffer[shidx] < shp.z);

        vec3f l = normalize(m2v( m_mvp*v2m(m_ldir) ));
        //vec3f eye = m2v(m_mvp*v2m(m_edir)).normalize();
        vec2i uv = var_uv[0]*bar.x + var_uv[1]*bar.y + var_uv[2]*bar.z;

        vec3f nrgb = 2.f*vec3f(m_norm.rgb(uv))/255.f - vec3f(1.f);
        vec3f n = normalize(m2v( m_mvp_invtr*v2m(nrgb) ));

        float diffuse = std::max(0.f, dot(n,l));

        float e = m_spec.lum(uv);
        vec3f r = 2.f*n*dot(n,l) - l; r.normalize();
        float specular = pow( std::max(0.f,r.z), e);

        vec3f texc = m_tex.rgb(uv);
        vec3f res_rgb = max(vec3f(5.f), shadow*texc*(diffuse + 0.6f*specular));
        //vec3f res_rgb = vec3f(255)*specular;
        color = vec4i(clamp_color(res_rgb), 255);

//        const int wsz = 30;
//        const int wwi = 2;
//        if ((uv.x % wsz) < wwi || (uv.y % wsz) < wwi) color = vec4i(255);
//        else return true;

        return false;
    }
};

} // namespace bacchus
