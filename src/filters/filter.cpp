#include "filter.h"

#include "glwrap/program.h"

namespace bacchus {

Filter::Filter(const std::string& vert, const std::string& frag)
    : mVertSrc(vert)
    , mFragSrc(frag)
    , ratio (1.0f)
    , mChanged(true)
{
    pass_0 = new Program(mVertSrc.c_str(), mFragSrc.c_str());
    pass_0->set_draw_mode(GL_TRIANGLE_FAN, 4);
    pass_0->set_out(nullptr);
    pass_0->vertex("posAttr") = new Vertex(gPosTex, 4, 2, 4, 0);
    pass_0->vertex("texAttr") = new Vertex(gPosTex, 4, 2, 4, 2);
    pass_0->parameter("matrix") = &mat;
    mat.set_scale(1.0f, 1.0f);
}

Filter::~Filter() {
    delete pass_0;
}

void Filter::onResize(int /*w*/, int /*h*/) {

}

void Filter::onInit() {

}

void Filter::onRender() {

}

void Filter::resize(int w, int h) {
    onResize(w,h);
    width = w;
    height = h;
    w>h ? mat.set_scale(ratio*h/float(w), 1.0f) : mat.set_scale(1.0f, w/float(ratio*h));
    pass_0->set_out_size(w, h);
    mChanged = true;
}

void Filter::init() {
    onInit();
    pass_0->createGl();
}

void Filter::render() {
    onRender();

    if (!mChanged)
        return;

    pass_0->render();
    mChanged = false;
}

void Filter::addParameter(const std::string& name, Value* value) {
    pass_0->parameter(name) = value;
}

void Filter::addTexture(const std::string &name, Texture* tex) {
    pass_0->add_texture(name, tex);
}

void Filter::setInput(Texture* inTex) {
    pass_0->add_texture("img", inTex);
}

void Filter::setOutput(FrameBuffer* outFbt) {
    pass_0->set_out(outFbt);
}

void Filter::setRatio(float new_ratio) {
    ratio = new_ratio;
}

} // namespace bacchus
