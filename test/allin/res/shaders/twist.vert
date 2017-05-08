attribute vec2 poscoord;
attribute vec2 texcoord;

uniform float time;

varying vec2 texCoord0;

const float angle_deg_max = 180.0;
const float height = 0.5;

vec4 twist(vec4 p, float t) {
    float ct = cos(t);
    float st = sin(t);
    vec4 newpos;
    newpos.x = p.x*ct - p.z*st;
    newpos.z = p.x*st + p.z*ct;
    newpos.y = p.y;
    newpos.w = p.w;
    return newpos;
}

void main() {
    float angle_deg = angle_deg_max*sin(time);
    float angle_rad = angle_deg * 3.14159 / 180.0;

    float ang = (height*0.5 + gl_Vertex.y)/height * angle_rad;

    vec4 twistedPosition = twist(ftransform(), ang);

    gl_Position = gl_ModelViewProjectionMatrix * twistedPosition;

    gl_TexCoord[0] = gl_MultiTexCoord0;
}
