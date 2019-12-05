#pragma once

namespace bacchus {

class FilterOES: public Filter {
public:
    FilterOES()
        : Filter(
              "#version 320 es\n"
              "in vec4 posAttr;\n"
              "in vec2 texAttr;\n"
              "out vec2 tex;\n"
              "uniform mat4 matrix;\n"
              "void main() {\n"
              "   tex = vec2(texAttr.x, 1.0-texAttr.y);\n"
              "   gl_Position = matrix * posAttr;\n"
              "}\n"
              ,
              "#version 320 es\n"
              "#extension GL_OES_EGL_image_external : require\n"
              "#extension GL_EXT_YUV_target : require\n"
              "precision mediump float;\n"
              "out vec4 fragColor;\n"
              "in vec2 tex;\n"
              "uniform sampler2D img;\n"
              "void main() {\n"
              "   fragColor = texture(img, tex);\n"
              "}\n"
              ) {}
};

} // namespace bacchus

