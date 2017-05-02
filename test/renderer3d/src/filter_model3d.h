#pragma once

#include <string>

#include "glwrap/values.h"

namespace bacchus {

class Program;
class Texture;
class FrameBufferTexture;
class Model;

class FilterModel3d {
public:
    FilterModel3d(const Model& model);
    virtual ~FilterModel3d();

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
    const Model& m_model;
    int width, height;
    float ratio;
    Program* pass_0;
    umat4f mat;
    bool mChanged;

    // ctx
    bool reflect = false;
    bool blend = false;
};

} // namespace bacchus
