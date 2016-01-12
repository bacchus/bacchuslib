#include "filter_gauss.h"

namespace bacchus {

std::string vertSrcGauss =
        "attribute vec4 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = texAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

std::string fragSrcGauss =
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "uniform vec2 size;\n"
        "uniform float kern[9];\n"
        "uniform float offs[18];\n"
        "void main() {\n"
        "   vec4 sum = vec4(0.0);\n"
        "   vec2 sxy = vec2(1.0/size.x, 1.0/size.y);\n"
        "   if (tex.x < 0.495) {\n"
        "       for (int i=0; i<9; ++i) {\n"
        "           vec4 tmp = texture2D(img, tex + sxy*vec2(offs[2*i], offs[2*i+1]));\n"
        "           sum += tmp*kern[i];\n"
        "       }\n"
        "   } else if (tex.x > 0.505) {\n"
        "       sum = texture2D(img, tex);\n"
        "   } else {\n"
        "       sum = vec4(0.3,0.5,0.8,1.0);\n"
        "   }\n"
        "   gl_FragColor = sum;\n"
        "}\n";

FilterGauss::FilterGauss()
    : Filter(vertSrcGauss, fragSrcGauss)
{
    m_sigma.x = 1;
    float fkern[9] = {
        1.f/16.f, 2.f/16.f, 1.f/16.f,
        2.f/16.f, 4.f/16.f, 2.f/16.f,
        1.f/16.f, 2.f/16.f, 1.f/16.f };

    float foffs[18] = {
        -1.f,-1.f, 0.0,-1.f, 1.f,-1.f,
        -1.f, 0.0, 0.0, 0.0, 1.f, 0.0,
        -1.f, 1.f, 0.0, 1.f, 1.f, 1.f };

    m_kern.set(9, fkern);
    m_offs.set(18, foffs);

    addParameter("size", &m_size);
    addParameter("kern", &m_kern);
    addParameter("offs", &m_offs);
}

void FilterGauss::onResize(int w, int h) {
    m_size.x = w;
    m_size.y = h;
}

void FilterGauss::setSigma(float sigma) {
    m_sigma.x = sigma;
}

} // namespace bacchus
