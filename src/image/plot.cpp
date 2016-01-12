#include "plot.h"

namespace bacchus {

void Plot::plotData(const bacchus::matxf &x, const bacchus::matxf &y, uint color) {
    uint m = x.m;
    assert(y.m == m);
    assert(x.n == 1);
    assert(y.n == 1);

    vec2f paint_size = m_paint.resolution();
    vec2f x_size = minmax(x);
    vec2f y_size = minmax(y);
    vec2f minsz(x_size.x,y_size.x);
    vec2f maxsz(x_size.y,y_size.y);
    vec2f koef = paint_size/(maxsz - minsz);

    for (uint i = 0; i < m; ++i) {
        vec2f p = koef*(vec2f(x[i],y[i]) - minsz);
        m_paint.draw_circle(p.x,p.y, 2, color);
    }
}

void Plot::plotData(const bacchus::matxf &x, uint color) {
    uint m = x.m;
    assert(x.n == 1);

    vec2f paint_size = m_paint.resolution();
    vec2f x_size = vec2f(0.f, (float)m);
    vec2f y_size = minmax(x);
    vec2f minsz(x_size.x,y_size.x);
    vec2f maxsz(x_size.y,y_size.y);
    vec2f koef = paint_size/(maxsz - minsz);

    //    for (uint i = 0; i < m; ++i) {
    //        vec2f p = koef*(vec2f((float)i, x[i]) - minsz);
    //        m_paint.draw_circle(p.x,p.y, 2, color);
    //    }

    for (uint i = 3; i < m; i+=3) {
        vec2f p0 = koef*(vec2f((float)i-3, x[i-3]) - minsz);
        vec2f p1 = koef*(vec2f((float)i-2, x[i-2]) - minsz);
        vec2f p2 = koef*(vec2f((float)i-1, x[i-1]) - minsz);
        vec2f p3 = koef*(vec2f((float)i, x[i]) - minsz);
        //m_paint.draw_line(p1,p2, color);
        m_paint.draw_bezier_curve(p0,p1,p2,p3, color);
    }
}

//void Plot::plotLine(float k, float b) {
//    vec2f paint_size = m_paint.resolution();
//}

} // namespace bacchus
