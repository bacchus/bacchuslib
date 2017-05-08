uniform sampler2D tex0;
uniform vec2 center;
uniform float scale;
uniform int iter;

varying vec2 texCoord0;

void main() {
    vec2 z, c;

    c.x = 1.3333*(texCoord0.x - 0.5) * scale - center.x;
    c.y = (gl_TexCoord[0].y - 0.5) * scale - center.y;

    int i;
    z = c;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    gl_FragColor = texture2D(tex0, vec2((i == iter ? 0.0 : float(i)) / 100.0, 0.5));
}
