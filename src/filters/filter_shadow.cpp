#include "filter_shadow.h"

namespace bacchus {

std::string vertSrc =
        "attribute vec4 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = texAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

//std::string fragSrc =
//        "varying vec2 tex;\n"
//        "uniform vec2 size;\n"
//        "uniform float dist;\n"
//        "uniform sampler2D img;\n"
//        "void main() {\n"
//        "   vec2 off = vec2(-1.0, 0.0);\n"
//        "   vec2 scl = vec2(0.5, 1.0);\n"
//        "   float res = 0.0;\n"
//        "   res += texture2D(img, tex).a;\n"
//        "   res += texture2D(img, tex+dist*1.0*off/size).a;\n"
//        "   res += texture2D(img, tex+dist*2.0*off/size).a;\n"
//        "   res += texture2D(img, tex+dist*3.0*off/size).a;\n"
//        "   res = res>0.0?1.0 : 0.0;"
//        "   gl_FragColor = vec4(vec3(0.5), res);\n"
//        "}\n";

std::string fragSrc =
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(img, tex);\n"
        "}\n";

FilterShadow::FilterShadow()
    : Filter(vertSrc, fragSrc)
{
    m_dist.x = 20;
//    addParameter("size", &m_size);
//    addParameter("dist", &m_dist);
}

void FilterShadow::onResize(int w, int h) {
    m_size.x = w;
    m_size.y = h;
}

void FilterShadow::setDist(float dist) {
    m_dist.x = dist;
}

} // namespace bacchus
