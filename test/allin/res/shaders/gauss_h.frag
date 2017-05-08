uniform sampler2D tex0;
uniform vec2 imgsize;
uniform int sigma;

varying vec2 texCoord0;

float gaus(float x) {
    float s = float(sigma);
    float res = 0.4*exp(-x*x/(2.0*s*s))/s;//0.39894 (dx/x=0.26%)
    return res;
}

void main(void) {
    vec4 sum = vec4(0.0);
    float gsum = 1.0;
    for (int i=-3*sigma; i<3*sigma; ++i) {
        float g = gaus(float(i));
        float dx = texCoord0.x+float(i)/float(imgsize.x);
        if (dx>0.0 && dx<1.0)
           sum += g*texture2D(tex0, vec2(dx, texCoord0.y));
        else
           gsum -= g;
    }
    gl_FragColor = sum/gsum;
}
