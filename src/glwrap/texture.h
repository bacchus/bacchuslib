#pragma once

#include "glinclude.h"
#include "math/mdefs.h"

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

    static void activate(uint index) {
        CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + index));
    }

    static void bind(uint texture_id) {
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, texture_id));
    }

    //    static void unbind() {
    //        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
    //    }

    Texture() {}

    Texture(uint glid, uint w, uint h)
        : id(glid)
        , m_width(w)
        , m_height(h)
    {}

    Texture(uint w, uint h, uchar* pixels = nullptr)
        : m_width(w)
        , m_height(h)
        , m_pixels(pixels)
    {}

    virtual ~Texture() {}

    void createGl() {
        CHECK_GL_ERROR(glGenTextures(1, &id));
        //glPixelStorei(GL_UNPACK_ALIGNMENT, GL_PACK_ALIGNMENT);
        //set_filter_wrap();
    }

    virtual void bind() {
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, id));
    }

    virtual void unbind() {
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void filter(int minMode = NEAREST, int maxMode = NEAREST) {
        m_min_mod = minMode;
        m_max_mod = maxMode;
    }

    void wrap(int s = GL_CLAMP_TO_EDGE, int t = GL_CLAMP_TO_EDGE) {
        m_wrap_s = s;
        m_wrap_t = t;
    }

    void format(uint fmt = GL_RGBA) {
        m_format = fmt;
    }

    void mipmap(bool use) {
        m_mipmap = use;
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

    virtual void pixels() {
        bind();
        CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_pixels));
        if (mipmap()) {
            CHECK_GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
        }
        set_filter_wrap();
        unbind();
    }

    void sub_pixels(int lvl, int x, int y) {
        bind();
        CHECK_GL_ERROR(glTexSubImage2D(GL_TEXTURE_2D, lvl, x, y, m_width, m_height, m_format, GL_UNSIGNED_BYTE, m_pixels));
        if (mipmap() && lvl==0) {
            CHECK_GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
        }
        unbind();
    }

    void attachto() {
        CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0));
    }

    uint width() const { return m_width; }
    uint height() const { return m_height; }

protected:
    bool mipmap() const {
        return m_mipmap && is_pow2(m_width) && is_pow2(m_height);
    }

    void set_filter_wrap() {
        // set filter
        if (mipmap()) {
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        } else {
            CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_mod));
        }
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_max_mod));
        // set wrap
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s));
        CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t));
    }

protected:
    uint id = 0;
    uint m_width = 0;
    uint m_height = 0;
    uint m_format = GL_RGBA;
    uchar* m_pixels = nullptr;
    bool m_mipmap = false;
    uint m_min_mod = NEAREST;
    uint m_max_mod = NEAREST;
    uint m_wrap_s = GL_CLAMP_TO_EDGE;
    uint m_wrap_t = GL_CLAMP_TO_EDGE;

    friend inline uint getId(const Texture& tex);
};

#ifdef ANDROID
class TextureOES: public Texture {
public:
    TextureOES(uint w, uint h, sp<GraphicBuffer> buff, EGLDisplay in_dpy)
        : Texture(w, h, nullptr)
        , dpy(in_dpy)
    {
        EGLClientBuffer clientBuffer = (EGLClientBuffer)buff->getNativeBuffer();
        img = eglCreateImageKHR(dpy, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuffer, 0);
        if (img == EGL_NO_IMAGE_KHR) {
            LOGE("Fail eglCreateImageKHR");
        }
    }

    virtual void bind() {
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_EXTERNAL_OES, id));
    }

    virtual void unbind() {
        CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0));
    }

    virtual void pixels() {
        bind();
        CHECK_GL_ERROR(glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)img));
        unbind();
    }

private:
    EGLImageKHR img;
    EGLDisplay dpy;
};
#endif // ANDROID

inline uint getId(const Texture& tex) {
    return tex.id;
}

} // namespace bacchus
