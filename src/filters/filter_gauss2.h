/*
#pragma once
#error "Fix me"
#include "values.h"
#include "program.h"
#include "matrix.h"
#include "texture.h"
#include "frame_buffer.h"

#include "image/png_rw.h"

class Blur {
public:
    Blur();
    void resize(int w, int h);
    void init();
    void render();

private:

    bacchus::Program* pass_1;
    bacchus::Program* pass_0;

    bacchus::FrameBufferTexture* fbt;

    bacchus::Texture* tex;

    png_image img;
    int m_frame;
    int width, height;

    bacchus::mat4f mat;
};
*/

