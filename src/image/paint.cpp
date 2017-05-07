#include "paint.h"
#include "image.h"
#include "font.h"

#include "math/cplx.h"
#include "math/vec4.h"

#include "Perlin_Noise/PerlinNoise-all.h"

namespace bacchus {

Paint::Paint(Image &img): m_img(img) { }

vec2i Paint::resolution() const {
    return m_img.resolution();
}

void Paint::draw_point(vec2i t0, uint color, int size) {
    int dx = size*0.5001f;
    for (int y = -dx; y <= dx; ++y) {
        for (int x = -dx; x <= dx; ++x) {
            m_img.pix(t0.x + x, t0.y + y) = color;
        }
    }
}

void Paint::draw_line(vec2i t0, vec2i t1, uint color) {
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
        if (steep)  m_img.pix(y,x) = color;
        else        m_img.pix(x,y) = color;
        err2 += derr2;
        if (err2 > dlt.x) {
            y += (t1.y>t0.y?1:-1);
            err2 -= 2*dlt.x;
        }
    }
}

void Paint::draw_triangle(vec2i t0, vec2i t1, vec2i t2, uint color) {
    if (t0.y==t1.y && t0.y==t2.y) return;// degenerate
    if (t0.y > t1.y) std::swap(t0,t1);
    if (t0.y > t2.y) std::swap(t0,t2);
    if (t1.y > t2.y) std::swap(t1,t2);
    int total_height = t2.y - t0.y;
    for (int y = t0.y; y <= t2.y; ++y) {
        bool half2 = (y>t1.y || t0.y==t1.y);
        int segment_height = (half2 ? t2.y-t1.y : t1.y-t0.y);
        float ka = (y - t0.y)/(float)total_height;
        float kb = (y - (half2 ? t1.y : t0.y))/(float)segment_height;
        vec2i a = t0 + ka*(t2-t0);
        vec2i b = (half2 ? t1 + kb*(t2-t1) : t0 + kb*(t1-t0));
        if (a.x > b.x) std::swap(a,b);
        for (int x = a.x; x < b.x; ++x) {
            m_img.pix(x, y) = color;
        }
    }
}

void Paint::draw_circle(int x, int y, int r, uint color) {
    int width = m_img.width;
    int height = m_img.height;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float dx = x - j;
            float dy = y - i;
            if (dx*dx+dy*dy<r*r) {
                m_img.pix(j,i) = color;
            } else {
                //m_img.pix(j,i) = lum2i(0);
            }
        }
    }
}

void Paint::draw_text(const char *str, vec2i t0, uint color) {
    font.print(m_img, str, t0, color);
}

void Paint::draw_border() {
    uint color = lum2i(0);
    int width = m_img.width;
    int height = m_img.height;
    for (int y = 0; y < height; ++y) {
        m_img.pix(0, y) = color;
        m_img.pix(width-1, y) = color;
    }
    for (int x = 0; x < width; ++x) {
        m_img.pix(x,0) = color;
        m_img.pix(x,height-1) = color;
    }
}

void Paint::draw_fractal_mandelbrot() {
    int width = m_img.width;
    int height = m_img.height;
    float xi = -1.9f;
    float xa = 0.5f;
    float yi = -1.2f;
    float ya = 1.2f;
    const int n = 100;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cplx z;
            int k;
            for (k = 0; k < n; ++k) {
                if (std::fabs(z) > 2)
                    break;
                cplx p(xi+(x+0.5f)*(xa-xi)/width, ya-(y+0.5f)*(ya-yi)/height);
                z = z*z + p;
            }
            float clr = (float)k/n;
            //uchar r = (uchar)(pow(clr,0.6f 0.3f 0.1f)*255.0f + 0.5f);
            //uchar g = (uchar)(log(clr)*255.0f + 0.5f);
            m_img.pix(x,y) = lum2i(255*clr);
        }
    }
}

uint iterate_mandel(float a, float b) {
    float x = 0.f;
    float y = 0.f;
    float x2 = 0.f;
    float y2 = 0.f;
    // f(z)=z^2+c
    for (int i = 0; i < 255; ++i) {
        y = 2.f*x*y + b;
        x = x2 - y2 + a;

        x2 = x*x;
        y2 = y*y;

        const float m2 = x2 + y2;
        if (m2 > 4.f)
            return lum2i(i);
    }
    return rgba2i(230,180,50);
}

//void Paint::draw_fractal_mandelbrot_eq() {
//    draw_fractal_mandelbrot_sse(m_img.dat32, m_img.width, m_img.height);
//    int width = m_img.width;
//    int height = m_img.height;
//    const float ixres = 1.f/width;
//    const float iyres = 1.f/height;
//    for (int y = 0; y < height; ++y) {
//        for (int x = 0; x < width; ++x) {
//            const float a = -2.25f + 3.00f*(float)x*ixres;
//            const float b =  1.12f - 2.24f*(float)y*iyres;
//            m_img.pix(x,y) = iterate_mandel(a,b);
//        }
//    }
//}

__m128i iterate_mandel_sse(__m128 a, __m128 b) {
    __m128 x, y, x2, y2,m2;
    __m128 cond, itr;

    __m128 num1 = _mm_set1_ps(1.f);
    __m128 num4 = _mm_set1_ps(4.f);

    itr = _mm_setzero_ps();
    cond = _mm_setzero_ps();
    x = _mm_setzero_ps();
    y = _mm_setzero_ps();
    x2 = _mm_setzero_ps();
    y2 = _mm_setzero_ps();

    // f(z)=z^2+c
    int i;
    for (i = 0; i < 255; ++i) {
        y = _mm_mul_ps(x,y);
        y = _mm_add_ps(_mm_add_ps(y,y), b);
        x = _mm_add_ps(_mm_sub_ps(x2,y2), a);

        x2 = _mm_mul_ps(x,x);
        y2 = _mm_mul_ps(y,y);
        m2 = _mm_add_ps(x2,y2);

        const __m128 gt = _mm_cmpgt_ps(m2, num4);
        cond = _mm_or_ps(cond, gt);

        itr = _mm_add_ps(itr, _mm_andnot_ps(cond, num1));
        if (_mm_movemask_ps(cond)==0.f) {
            i = 255;
        }
    }
    const __m128i bb = _mm_cvtps_epi32(itr);
    const __m128i gg = _mm_slli_si128(bb, 1);
    const __m128i rr = _mm_slli_si128(bb, 2);
    const __m128i color = _mm_or_si128(_mm_or_si128(rr,gg),bb);
    return color;
}

void Paint::draw_fractal_mandelbrot_sse(uint* data, int width, int height) {
    __m128i* buff4 = reinterpret_cast<__m128i*>(data);

    const __m128 ixres = _mm_set1_ps(1.f/width);
    const __m128 iyres = _mm_set1_ps(1.f/height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x+=4) {
            __m128 a,b;
            a = _mm_set_ps(x+3, x+2, x+1, x+0);
            a = _mm_mul_ps(a, ixres);
            a = _mm_mul_ps(a, _mm_set1_ps( 3.00f));
            a = _mm_add_ps(a, _mm_set1_ps(-2.25f));

            b = _mm_set1_ps((float)y);
            b = _mm_mul_ps(b, iyres);
            b = _mm_mul_ps(b, _mm_set1_ps(-2.24f));
            b = _mm_add_ps(b, _mm_set1_ps( 1.12f));

            _mm_store_si128(buff4++, iterate_mandel_sse(a,b));
        }
    }
}

std::vector<vec2i> Paint::replot_xkcd(const std::vector<vec2i>& line, float radius, float step) {
    rndmz();
    float accuracy = 0.25f;
    std::vector<vec2i> reploted;
    vec2f begin = line[0];
    for (uint i = 1; i < line.size(); ++i) {
        vec2f p = line[i];
        vec2f delta = p - begin;
        float d = delta.length();
        if (d < step*(1-accuracy) && (i+1 < line.size())) {
            // too short
            continue;
        }

        if (d > step*(1-accuracy)) {
            // too long
            int n = d/step;
            for (int j = 0; j < n; ++j) {
                reploted.push_back(begin + delta*(float)j/(float)n);
            }
        }

        reploted.push_back(p);
        begin = p;
    }

    for (auto& p: reploted) {
        p += radius*vec2f(rnd() - 0.5f);
    }

    return reploted;
}

void Paint::draw_curve(const std::vector<vec2i>& line, uint color) {
    std::vector<vec2i> reploted = replot_xkcd(line, 5, 10);
    for (uint i = 0; i < reploted.size()-3; i+=3) {
        //draw_line(reploted[i], reploted[i+1], color);
        draw_bezier_curve(reploted[i],reploted[i+1],reploted[i+2],reploted[i+3], color);
    }
}

void Paint::draw_bezier_curve(vec2i t1, vec2i t2, vec2i t3, vec2i t4, uint color) {
    bool steep = false;
    if (std::abs(t1.x-t4.x) < std::abs(t1.y-t4.y)) {
        std::swap(t1.x, t1.y);
        std::swap(t2.x, t2.y);
        std::swap(t3.x, t3.y);
        std::swap(t4.x, t4.y);
        steep = true;
    }
    if (t1.x > t4.x) {
        std::swap(t1, t4);
        std::swap(t2, t3);
    }

    for (int x=t1.x; x<=t4.x; x++) {
        float t = (x-t1.x)/(float)(t4.x-t1.x);
        int y =   (1.f-t)*(1.f-t)*(1.f-t)*t1.y
                + 3.f*t*(1.f-t)*(1.f-t)*t2.y
                + 3.f*t*t*(1.f-t)*t3.y
                + t*t*t*t4.y;
        if (steep)  draw_point(vec2i(y,x),color,2);
        else        draw_point(vec2i(x,y),color,2);
        //if (steep)  m_img.pix(y,x) = color;
        //else        m_img.pix(x,y) = color;
    }
}

void Paint::draw_perlin_noise(float scale, float z) {
    uint width = m_img.width;
    uint height = m_img.height;
    uint seed = 237;
    PerlinNoise pn(seed);
    for (uint y = 0; y < height; ++y) {
        for (uint x = 0; x < width; ++x) {
            float dx = (float)x/(float)width;
            float dy = (float)y/(float)height;
            float n = pn.noise(scale*dx, scale*dy, z);
            //wood
            //float n = 20*pn.noise(10*dx, 10*dy, z);
            //n = n - std::floor(n);

            m_img.pix(x,y) = lum2i(std::floor(255*n));
        }
    }
}

} // namespace bacchus
