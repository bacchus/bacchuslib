#pragma once

#include <vector>

#include "image/image.h"

#include "math/transform.h"

namespace bacchus {

class IShader {
public:
    virtual ~IShader() {}
    virtual vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(const vec3f& bar, vec4i& color) = 0;
};

class Renderer3d {
    Image& m_img;
    mat4f m_model_view;
    mat4f m_viewport;
    mat4f m_projection;
public:
    Renderer3d(Image& img): m_img(img) {
        assert(m_img.valid());
    }

    void set_viewport(float x, float y, float w, float h, float depth) {
        m_viewport = viewport(x,y, w,h, depth);
    }

    void set_projection(float coef) {
        m_projection = projection(coef);
    }

    void set_modelview(const vec3f& eye, const vec3f& center, const vec3f& up) {
        m_model_view = lookat(eye, center, up);
    }

    mat4f mvp() const {
        return m_projection*m_model_view;
    }

    void triangle(vec4f* clipc, IShader& shader, int *zbuffer) {
        vec4f pts[3];
        vec3f pts2[3];
        for (int i = 0; i < 3; ++i) {
            pts[i] = m_viewport*clipc[i];
            pts2[i] = m2v(pts[i]);
        }

        vec2f imgclamp(m_img.width-1, m_img.height-1);
        vec2f bboxmin(imgclamp);
        vec2f bboxmax(0, 0);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                bboxmin[j] = std::max(0.f, std::min(pts2[i][j], bboxmin[j]));
                bboxmax[j] = std::min(imgclamp[j], std::max(pts2[i][j], bboxmax[j]));
            }
        }
        vec3i p;
        for (p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x) {
            for (p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y) {
                vec3f bc_screen = barycentric(pts2, p);
                vec3f bc_clip = bc_screen/vec3f(pts[0][3],pts[1][3],pts[2][3]);
                bc_clip /= dot(bc_clip, vec3f(1.f));
                float z = pts[0][2]*bc_screen.x + pts[1][2]*bc_screen.y + pts[2][2]*bc_screen.z;
                float w = pts[0][3]*bc_screen.x + pts[1][3]*bc_screen.y + pts[2][3]*bc_screen.z;
                p.z = 0.5f + z/w;
                //p.z = std::max(0.f, std::min(255.f, pts2[0][2]*bc_screen.x + pts2[1][2]*bc_screen.y + pts2[2][2]*bc_screen.z + 0.5f));
                //p.z = dot(clipc[2].xyz(), bc_clip);
                int idx = p.x + p.y*m_img.width;
                if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0 || p.z < zbuffer[idx])
                    continue;
                vec4i color(0);
                if (!shader.fragment(bc_clip, color)) {
                    zbuffer[idx] = p.z;
                    //TODO: we need format/bytespp independent alg -> color type
                    //TODO: is it possible to set pixel without rgb2i
                    m_img.pix(p.xy()) = rgba2i(color);
                }
            }
        }
    }

    void triangle_wire(vec4f* ptsin, const vec4i& color) {
        vec3f pts[3];
        for (int i = 0; i < 3; ++i) {
            pts[i] = m2v(m_viewport*ptsin[i]);
        }

        line(pts[0].xy(), pts[1].xy(), color);
        line(pts[1].xy(), pts[2].xy(), color);
        line(pts[2].xy(), pts[0].xy(), color);
    }

protected:
    void line(vec2i t0, vec2i t1, const vec4i& color) {
        uint col4 = rgba2i(color);
        bool steep = false;
        if (std::abs(t0.x-t1.x) < std::abs(t0.y-t1.y)) {
            std::swap(t0.x, t0.y);
            std::swap(t1.x, t1.y);
            steep = true;
        }
        if (t0.x > t1.x) std::swap(t0, t1);
        vec2i dlt = t1 - t0;
        int derr2 = 2*std::abs(dlt.y);
        int err2 = 0;
        int y = t0.y;
        for (int x = t0.x; x <= t1.x; ++x) {
            if (steep)  m_img.pix(y,x) = col4;
            else        m_img.pix(x,y) = col4;
            err2 += derr2;
            if (err2 > dlt.x) {
                y += (t1.y>t0.y?1:-1);
                err2 -= 2*dlt.x;
            }
        }
    }

    vec3f barycentric(vec3f* pts, vec3f p) {
        vec3f u = cross(
                vec3f(pts[2][0]-pts[0][0], pts[1][0]-pts[0][0], pts[0][0]-p[0]),
                vec3f(pts[2][1]-pts[0][1], pts[1][1]-pts[0][1], pts[0][1]-p[1]));
        if (std::abs(u[2]) > 0.01)
            return vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
        return vec3f(-1,1,1);
    }

    //    void triangle(vec3i t0, vec3i t1, vec3i t2, vec3i it0, vec3i it1, vec3i it2, int *zbuffer) {
    //        if (t0.y==t1.y && t0.y==t2.y) return;// degenerate
    //        if (t0.y > t1.y) { std::swap(t0,t1); std::swap(it0,it1); }
    //        if (t0.y > t2.y) { std::swap(t0,t2); std::swap(it0,it2); }
    //        if (t1.y > t2.y) { std::swap(t1,t2); std::swap(it1,it2); }
    //        int total_height = t2.y - t0.y;
    //        for (int y = t0.y; y <= t2.y; ++y) {
    //            bool half2 = (y>t1.y || t0.y==t1.y);
    //            int segment_height = (half2 ? t2.y-t1.y : t1.y-t0.y);
    //            float ka = (y - t0.y)/(float)total_height;
    //            float kb = (y - (half2 ? t1.y : t0.y))/(float)segment_height;
    //            vec3i a = t0 + ka*(t2-t0);
    //            vec3i b = (half2 ? t1 + kb*(t2-t1) : t0 + kb*(t1-t0));
    //            vec3f ita = it0 + ka*(it2-it0);
    //            vec3f itb = (half2 ? it1 + kb*(it2-it1) : it0 + kb*(it1-it0));
    //            if (a.x > b.x) { std::swap(a,b); std::swap(ita,itb);}
    //            for (int x = a.x; x < b.x; ++x) {
    //                float phi = (a.x==b.x ? 1.f : (float)(x - a.x)/(float)(b.x - a.x));
    //                int z = a.z + phi*(b.z - a.z);
    //                vec3f itp = ita + phi*(itb - ita);
    //                int idx = x + y*m_img.width;
    //                if (z > zbuffer[idx]) {
    //                    zbuffer[idx] = z;
    //                    m_img.pix(x, y) = rgba2i(itp.x, itp.y, itp.z);
    //                }
    //            }
    //        }
    //    }

    //    void triangle(vec4f* ptsin, vec2i* uvs, const Image& tex, int *zbuffer) {
    //        vec3i pts[3];
    //        for (int i = 0; i < 3; ++i) {
    //            pts[i] = m2v(ptsin[i]);
    //        }

    //        vec2i bboxmin(m_img.width-1, m_img.height-1);
    //        vec2i bboxmax(0, 0);
    //        for (int i = 0; i < 3; ++i) {
    //            for (int j = 0; j < 2; ++j) {
    //                bboxmin[j] = std::min(pts[i][j], bboxmin[j]);
    //                bboxmax[j] = std::max(pts[i][j], bboxmax[j]);
    //            }
    //        }
    //        vec3i p;
    //        for (p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x) {
    //            for (p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y) {
    //                vec3f c = barycentric(pts, p);
    //                p.z = std::max(0.f, std::min(255.f, pts[0][2]*c.x + pts[1][2]*c.y + pts[2][2]*c.z + 0.5f));
    //                int idx = p.x + p.y*m_img.width;
    //                if (c.x < 0 || c.y < 0 || c.z < 0 || p.z < zbuffer[idx])
    //                    continue;
    //                zbuffer[idx] = p.z;
    //                vec2i uv = uvs[0]*c.x + uvs[1]*c.y + uvs[2]*c.z;
    //                m_img.pix(p.xy()) = rgba2i(tex.rgba(uv));
    //            }
    //        }
    //    }


    //    void triangle(vec3i* pts, vec3i* colors, int *zbuffer) {
    //        vec2i bboxmin(m_img.width-1, m_img.height-1);
    //        vec2i bboxmax(0, 0);
    //        for (int i = 0; i < 3; ++i) {
    //            for (int j = 0; j < 2; ++j) {
    //                bboxmin[j] = std::min(pts[i][j], bboxmin[j]);
    //                bboxmax[j] = std::max(pts[i][j], bboxmax[j]);
    //            }
    //        }
    //        vec3i p;
    //        for (p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x) {
    //            for (p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y) {
    //                vec3f c = barycentric(pts, p);
    //                p.z = std::max(0.f, std::min(255.f, pts[0][2]*c.x + pts[1][2]*c.y + pts[2][2]*c.z + 0.5f));
    //                int idx = p.x + p.y*m_img.width;
    //                if (c.x < 0 || c.y < 0 || c.z < 0 || p.z < zbuffer[idx])
    //                    continue;
    //                zbuffer[idx] = p.z;
    //                vec3i color = colors[0]*c.x + colors[1]*c.y + colors[2]*c.z;
    //                m_img.pix(p.xy()) = rgba2i(color.x, color.y, color.z);
    //            }
    //        }
    //    }

    //    void triangle_affine_notcorrect(vec4f* ptsin, IShader& shader, int *zbuffer) {
    //        vec3f pts[3];
    //        for (int i = 0; i < 3; ++i) {
    //            pts[i] = m2v(ptsin[i]);
    //        }
    //        vec2f imgclamp(m_img.width-1, m_img.height-1);
    //        vec2f bboxmin(imgclamp);
    //        vec2f bboxmax(0, 0);
    //        for (int i = 0; i < 3; ++i) {
    //            for (int j = 0; j < 2; ++j) {
    //                bboxmin[j] = std::max(0.f, std::min(pts[i][j], bboxmin[j]));
    //                bboxmax[j] = std::min(imgclamp[j], std::max(pts[i][j], bboxmax[j]));
    //            }
    //        }
    //        vec3i p;
    //        for (p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x) {
    //            for (p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y) {
    //                vec3f c = barycentric(pts, p);
    //                p.z = std::max(0.f, std::min(255.f, pts[0][2]*c.x + pts[1][2]*c.y + pts[2][2]*c.z + 0.5f));
    //                int idx = p.x + p.y*m_img.width;
    //                if (c.x < 0 || c.y < 0 || c.z < 0 || p.z < zbuffer[idx])
    //                    continue;
    //                vec4i color(0);
    //                if (!shader.fragment(c, color)) {
    //                    zbuffer[idx] = p.z;
    //                    m_img.pix(p.xy()) = rgba2i(color);
    //                }
    //            }
    //        }
    //    }
};

} // namespace bacchus
