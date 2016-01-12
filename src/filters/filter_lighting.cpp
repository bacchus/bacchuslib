#include "filter_lighting.h"

namespace bacchus {

std::string vertSrcLight =
        "attribute vec4 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "varying vec2 pos;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = texAttr;\n"
        "   vec4 p = matrix * posAttr;\n"
        "   pos = p.xy;\n"
        "   gl_Position = p;\n"
        "}\n";

std::string fragSrcLight =
        "varying vec2 tex;\n"
        "varying vec2 pos;\n"
        "uniform vec2 size;\n"
        "uniform sampler2D img;\n"
        "uniform sampler2D img_norm;\n"
        "uniform vec4 cam;\n"
        "uniform vec4 cdf;\n"
//        "uniform vec4 csp;\n"
//        "uniform vec3 lpos;\n"

        "void main() {\n"
        "   vec3 eyevec = vec3(0,0,-1);\n"
        "   vec4 sam = vec4(0.2,0.2,0.2,1.0);\n"
        "   vec4 cdf = vec4(0.7,1.0,0.0,1.0);\n"
        "   vec4 csp = vec4(0.7,0.7,0.0,1.0);\n"
        "   vec3 lpos = vec3(1.5,1.5,5.0);\n"
        "   vec3 N = normalize(texture2D(img_norm, tex).rgb);\n"
        "   vec3 L = normalize(lpos - vec3(pos, 0.0));\n"
        "   float lambert = max(dot(N, L), 0.0);\n"
        "   vec4 sdf = 0.0*cdf*lambert;\n"
        "   float shininess = 2.0;\n"
        "   vec3 R = reflect(-L, N);\n"
        "   vec3 E = normalize(eyevec);\n"
        "   vec4 ssp = csp*pow( max(dot(R,E),0.0), shininess );\n"
        "   gl_FragColor = clamp(sam + sdf*texture2D(img, tex) + ssp, 0.0, 1.0);\n"
        "}\n";

FilterLighting::FilterLighting()
    : Filter(vertSrcLight, fragSrcLight)
{

//    addParameter("size", &m_size);
//    addParameter("cam", &m_cam);
//    addParameter("cdf", &m_cdf);
//    addParameter("lpos", &m_lpos);
}

void FilterLighting::onResize(int w, int h) {
    m_size.x = w;
    m_size.y = h;
}

void FilterLighting::setSigma(float sigma) {
    m_sigma.x = sigma;
}

} // namespace bacchus
