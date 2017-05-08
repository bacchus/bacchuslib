varying vec2 tex;
uniform sampler2D img;
void main() {
   gl_FragColor = texture2D(img, tex);
}
