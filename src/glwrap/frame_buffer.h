#pragma once

#include <GLES3/gl3.h>

#include "texture.h"

namespace bacchus {

class FrameBuffer {
public:

    static void bind(int framebuffer_id) {
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));
    }

    static void unbind() {
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    FrameBuffer(): id(0) {}
    FrameBuffer(uint w, uint h)
        : id(0)
        , m_width(w)
        , m_height(h)
    {}

    explicit FrameBuffer(int glid): id(glid) {}
    ~FrameBuffer() {}

    void createGl() {
        CHECK_GL_ERROR(glGenFramebuffers(1, &id));
    }

    void bind() {
        CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, id));
    }

    void destroyGl() {
        CHECK_GL_ERROR(glDeleteFramebuffers(1, &id));
    }

    void attach(Texture* tex) {
        tex->attachto();
        //CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.id, 0));
    }

    uint width() const { return m_width; }
    uint height() const { return m_height; }

private:
    uint id;
    uint m_width;
    uint m_height;

    friend inline uint getId(const FrameBuffer& fb);
};

class FrameBufferTexture: public FrameBuffer {
public:
    FrameBufferTexture(uint w, uint h) {
        fb_tex = new Texture(w, h);
    }

    FrameBufferTexture(int glid, Texture* tex)
        : FrameBuffer(glid)
        , fb_tex(tex)
    {
    }

    ~FrameBufferTexture() {
        if (fb_tex)
            delete fb_tex;
    }

    void createGl() {
        fb_tex->createGl();
        fb_tex->filter(Texture::LINEAR, Texture::LINEAR);
        fb_tex->bind();
        fb_tex->pixels();
        FrameBuffer::createGl();
        bind();
        attach(fb_tex);
        unbind();
        fb_tex->unbind();
    }

    Texture* out() {
        return fb_tex;
    }

    uint width() { return fb_tex->width(); }
    uint height() { return fb_tex->height(); }

private:
    Texture* fb_tex;
};

inline uint getId(const FrameBuffer& fb) {
    return fb.id;
}

} // namespace bacchus