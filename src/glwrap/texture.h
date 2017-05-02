#pragma once

#include "glinclude.h"

namespace bacchus {

class Texture {
public:

    enum TexFilter {
        NEAREST= GL_NEAREST,
        LINEAR = GL_LINEAR,
    };

    enum TexWrap {
        REPEAT = GL_REPEAT,
        MIRROR = GL_MIRRORED_REPEAT,
        CLAMP  = GL_CLAMP_TO_EDGE,
    };

    static void activate(int index) {
        glActiveTexture(GL_TEXTURE0 + index);
    }

    static void bind(int texture_id) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }

    static void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture()
        : m_width(0)
        , m_height(0)
        , m_format(GL_RGBA)
        , m_pixels(NULL)
    {}

    Texture(int glid, int w, int h)
        : id(glid)
        , m_width(w)
        , m_height(h)
        , m_format(GL_RGBA)
        , m_pixels(NULL)
    {}

    Texture(uint w, uint h, uchar* pixels = NULL)
        : m_width(w)
        , m_height(h)
        , m_format(GL_RGBA)
        , m_pixels(pixels)
    {}

    ~Texture() {}

    void createGl() {
        CHECK_GL_ERROR(glGenTextures(1, &id));
        filter();
        wrap();
    }

    void bind() {
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, id));
    }

    void filter(int minMode=NEAREST, int maxMode=NEAREST) {
        bind();
        CHECK_GL_ERROR(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMode));
        CHECK_GL_ERROR(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxMode));
        unbind();
    }

    void wrap(int s=GL_CLAMP_TO_EDGE, int t=GL_CLAMP_TO_EDGE) {
        bind();
        CHECK_GL_ERROR(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s));
        CHECK_GL_ERROR(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t));
        unbind();
    }

    void format(uint fmt = GL_RGBA) {
        m_format = fmt;
    }

    void destroyGl() {
        CHECK_GL_ERROR(glDeleteTextures(1, &id));
    }

    void pixels(uint w, uint h, uchar* data) {
        m_width = w;
        m_height = h;
        m_pixels = data;
        pixels();
    }

    void pixels() {
        bind();
        CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_pixels));
        unbind();
    }

    void attachto() {
        CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0));
    }

    uint width() const { return m_width; }
    uint height() const { return m_height; }

private:
    uint id;
    uint m_width;
    uint m_height;
    uint m_format;
    uchar* m_pixels;

    friend inline uint getId(const Texture& tex);
};

inline uint getId(const Texture& tex) {
    return tex.id;
}

} // namespace bacchus
