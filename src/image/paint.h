#pragma once

#include <vector>

#include "math/mdefs.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "font.h"

namespace bacchus {

class Image;

class Paint {
public:
    Paint(Image& img);

    vec2i resolution() const;

    void draw_point(vec2i t0, uint color, int size=1);
    void draw_line(vec2i t0, vec2i t1, uint color);
    void draw_triangle(vec2i t0, vec2i t1, vec2i t2, uint color);
    void draw_circle(int x, int y, int r, uint color);
    void draw_text(const char* str, vec2i t0, uint color);
    void draw_border();
    void draw_fractal_mandelbrot();
    void draw_fractal_mandelbrot_eq();
    void draw_fractal_mandelbrot_sse(uint *data, int w, int height);

    void draw_curve(const std::vector<vec2i> &line, uint color);
    void draw_bezier_curve(vec2i t1, vec2i t2, vec2i t3, vec2i t4, uint color);

    void draw_perlin_noise(double z = 0.8);

public:
    // colors
    enum Color {
        black = 0xFF000000, //static const uint black = rgba2i(0,0,0);
        white = 0xFFFFFFFF, //rgba2i(255,255,255)
        red = 0xFF0000FF,   //rgba2i(255,0,0)
        green = 0xFF00FF00, //rgba2i(0,255,0),
        blue = 0xFFFF0000,  //rgba2i(0,0,255),
        orange = 0xFF64B4FA //rgba2i(250,180,100)
    };

private:
    std::vector<vec2i> replot_xkcd(const std::vector<vec2i>& line, float radius, float step);

private:
    Image& m_img;
    Font font;
};

} // namespace bacchus
