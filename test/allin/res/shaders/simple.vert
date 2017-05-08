attribute vec4 posAttr;
attribute vec2 texAttr;
varying vec2 tex;
uniform mat4 matrix;
void main() {
   tex = texAttr;
   gl_Position = matrix * posAttr;
}
