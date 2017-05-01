#pragma once

#include <string>

#include "glwrap/values.h"

namespace bacchus {

class Program;
class Texture;
class FrameBufferTexture;

//static const char* gDefVertSrc =
//        "attribute vec4 posAttr;\n"
//        "attribute vec2 texAttr;\n"
//        "varying vec2 tex;\n"
//        "uniform mat4 matrix;\n"
//        "void main() {\n"
//        "   tex = texAttr;\n"
//        "   gl_Position = matrix * posAttr;\n"
//        "}\n";

//static const char* gDefFragSrc =
//        "varying vec2 tex;\n"
//        "uniform sampler2D img;\n"
//        "void main() {\n"
//        "   gl_FragColor = texture2D(img, tex);\n"
//        "}\n";

static const float gPosTex[] = {
    -1.0f, -1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f,    0.0f, 1.0f,
    1.0f, 1.0f,     1.0f, 1.0f,
    1.0f, -1.0f,    1.0f, 0.0f
};

class Filter {
public:
    Filter(const std::string& vert, const std::string& frag);
    virtual ~Filter();

    virtual void onResize(int w, int h);
    virtual void onInit();
    virtual void onRender();

    virtual void resize(int w, int h);
    virtual void init();
    virtual void render();
    virtual void addParameter(const std::string& name, Value* value);
    virtual void addTexture(const std::string& name, Texture* tex);

    virtual void setInput(Texture* inTex);
    virtual void setOutput(FrameBufferTexture* outFbt);

    void setChanged() { mChanged = true; }
    void setRatio(float new_ratio); // w/h

protected:
    int width, height;
    float ratio;
    std::string mVertSrc;
    std::string mFragSrc;
    Program* pass_0;
    umat4f mat;
    bool mChanged;
};

} // namespace bacchus
