/*
#error "Fix me"
#include "blur.h"

static const char *def_vert_src =
        "attribute vec4 posAttr;\n"
        "attribute vec2 texAttr;\n"
        "varying vec2 tex;\n"
        "uniform mat4 matrix;\n"
        "void main() {\n"
        "   tex = texAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *frag_tex_src =
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(img, tex);\n"
        "}\n";

static const char *frag_tex_gbra_src =
        "varying vec2 tex;\n"
        "uniform sampler2D img;\n"
        "void main() {\n"
        "   gl_FragColor = texture2D(img, tex).bgra;\n"
        "}\n";

static const float qpostex[] = {
    -1.0f, -1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f,    0.0f, 1.0f,
    1.0f, 1.0f,     1.0f, 1.0f,
    1.0f, -1.0f,    1.0f, 0.0f
};

Blur::Blur() {
    m_frame = 0;
    read_png_mirrored("test.png", img);

    tex = new bacchus::Texture(img.width, img.height, img.data);
    fbt = new bacchus::FrameBufferTexture(100, 100);

    pass_0 = new bacchus::Program(def_vert_src, frag_tex_src);
    pass_0->set_draw_mode(GL_TRIANGLE_FAN, 4);
    pass_0->set_out(fbt);
    pass_0->vertex("posAttr") = new bacchus::Vertex(qpostex, 4, 2, 4, 0);
    pass_0->vertex("texAttr") = new bacchus::Vertex(qpostex, 4, 2, 4, 2);
    pass_0->parameter("matrix") = new bacchus::mat4f();
    pass_0->add_texture("img", tex);

    pass_1 = new bacchus::Program(def_vert_src, frag_tex_gbra_src);
    pass_1->set_draw_mode(GL_TRIANGLE_FAN, 4);
    pass_1->set_out(nullptr);
    pass_1->vertex("posAttr") = new bacchus::Vertex(qpostex, 4, 2, 4, 0);
    pass_1->vertex("texAttr") = new bacchus::Vertex(qpostex, 4, 2, 4, 2);
    pass_1->parameter("matrix") = &mat;
    pass_1->add_texture("img", fbt->out());
}

void Blur::resize(int w, int h) {
    width = w;
    height = h;

    static bool once = true;
    if (once) {
        mat.scale(1.0, (float)width/(float)height);
        once = false;
    }

    pass_1->set_out_size(w, h);
}

void Blur::init() {
    tex->createGl();
    tex->pixels();
    fbt->createGl();

    pass_0->createGl();
    pass_1->createGl();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.7,0.5,0.2,1.0);
}

void Blur::render() {
    pass_0->render();
    pass_1->render();

    ++m_frame;
    //mat.rotate(1);
}
*/

