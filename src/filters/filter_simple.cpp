#include "filter_simple.h"

namespace bacchus {

static std::string vertSrc =
        "attribute vec4 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = texAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

//"uniform float data;n\"data*
static std::string fragSrc =
        "precision mediump float;\n"
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(img, tex);\n"
        "}\n";

FilterSimple::FilterSimple()
    : Filter(vertSrc, fragSrc)
{
    m_data.x = 1.0f;
    //    addParameter("size", &m_size);
//        addParameter("data", &m_data);
}

void FilterSimple::setData(float data) {
    m_data.x = data;
    setChanged();
}

void FilterSimple::onResize(int w, int h) {
    m_size.x = w;
    m_size.y = h;
}

} // namespace bacchus
