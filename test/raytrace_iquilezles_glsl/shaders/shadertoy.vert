attribute vec4 posAttr;
varying vec4 pos;
uniform mat4 matrix;
void main() {
   pos = matrix * posAttr;
   gl_Position = pos;
}
