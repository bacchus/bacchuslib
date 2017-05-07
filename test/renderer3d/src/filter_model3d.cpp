#include "filter_model3d.h"
#include "raytrace/obj_model.h"
#include "glwrap/program.h"
#include "math/transform.h"

namespace bacchus {

static std::string vertSrc =
        "attribute vec3 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = vec2(1.0 - texAttr.x,1.0 - texAttr.y);\n"
        "   gl_Position = matrix * vec4(posAttr, 1.0);\n"
        "}\n";

//"uniform float data;n\"data*
static std::string fragSrc =
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(img, tex);\n"
        "}\n";

FilterModel3d::FilterModel3d(const Model &model)
    : m_model(model)
    , ratio (1.0f)
    , mChanged(true)
{
    pass_0 = new Program(vertSrc.c_str(), fragSrc.c_str());
    pass_0->set_out(nullptr);

    // add Textures from Model
    // mb Normal Map texture

    pass_0->vertex("posAttr") = new Vertex(m_model.mesh(), m_model.mesh_size(), 3, 8, 0);
    pass_0->vertex("texAttr") = new Vertex(m_model.mesh(), m_model.mesh_size(), 2, 8, 3);
    pass_0->set_draw_mode(GL_TRIANGLES, m_model.mesh_size());

    pass_0->parameter("matrix") = &mat;

    //mat = viewport(0,0,width,height,1);
    mat = lookat(m_model.center() + 2.0f*m_model.radius()*vec3f(0,0,1)
                         , m_model.center(), vec3f(0,1,0));
    //mat.set_scale (1.0f, 1.0f);
}

FilterModel3d::~FilterModel3d() {
    delete pass_0;
}

void FilterModel3d::onResize(int /*w*/, int /*h*/) {

}

void FilterModel3d::onInit() {

}

void FilterModel3d::onRender() {
    CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
    CHECK_GL_ERROR(glCullFace(GL_BACK));
//    CHECK_GL_ERROR(glCullFace(GL_FRONT));

//    CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
//    CHECK_GL_ERROR(glDepthFunc(GL_LESS));

//    CHECK_GL_ERROR(glEnable(GL_BLEND));
//    CHECK_GL_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//    CHECK_GL_ERROR(glDepthMask(GL_FALSE));

//    CHECK_GL_ERROR(glDisable(GL_BLEND));
//    CHECK_GL_ERROR(glDepthMask(GL_TRUE));
}

void FilterModel3d::resize(int w, int h) {
    onResize(w,h);
    width = w;
    height = h;
    //w>h ? mat.set_scale(ratio*h/float(w), 1.0f) : mat.set_scale(1.0f, w/float(ratio*h));
    pass_0->set_out_size(w, h);
    mChanged = true;
}

void FilterModel3d::init() {
    onInit();
    pass_0->createGl();
}

void FilterModel3d::render() {
    onRender();

    if (!mChanged)
        return;

    pass_0->render();
    mChanged = false;
}

void FilterModel3d::addParameter(const std::string& name, Value* value) {
    pass_0->parameter(name) = value;
}

void FilterModel3d::addTexture(const std::string &name, Texture* tex) {
    pass_0->add_texture(name, tex);
}

void FilterModel3d::setInput(Texture* inTex) {
    pass_0->add_texture("img", inTex);
}

void FilterModel3d::setOutput(FrameBufferTexture* outFbt) {
    pass_0->set_out(outFbt);
}

void FilterModel3d::setRatio(float new_ratio) {
    ratio = new_ratio;
}

void FilterModel3d::setMat(const mat4f& new_mat) {
    mat = new_mat;
    mChanged = true;
}

} // namespace bacchus
