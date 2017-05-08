uniform float time;
uniform float frequency;

varying vec3 vNormal;
varying vec4 vVertex;

vec3 computeNormal( vec3 pos,
                    vec3 tangent,
                    vec3 binormal,
                    float phase,
                    float freq )
{
    mat3 J;

    float dist = sqrt(pos.x*pos.x + pos.z*pos.z);
    float jac_coef = cos(freq*dist + phase) / (dist+0.00001);

    // A matrix is an array of column vectors so J[2] is
    // the third column of J.

    J[0][0] = 1.0;
    J[0][1] = jac_coef * pos.x;
    J[0][2] = 0.0;

    J[1][0] = 0.0;
    J[1][1] = 1.0;
    J[1][2] = 0.0;

    J[2][0] = 0.0;
    J[2][1] = jac_coef * pos.z;
    J[2][2] = 1.0;

    vec3 u = J * tangent;
    vec3 v = J * binormal;

    vec3 n = cross(v, u);
    return normalize(n);
}

vec4 displaceVertexFunc( vec4 pos, float phase, float frequency )
{
    vec4 new_pos;

    new_pos.x = pos.x;
    new_pos.z = pos.z;
    new_pos.w = pos.w;

    float dist = sqrt(pos.x*pos.x + pos.z*pos.z);
    new_pos.y = pos.y + 20.0 * sin( frequency * dist + phase );

    return new_pos;
}

void main(void)
{
    vec4 displacedVertex;
    vec3 displacedNormal;

    // 1 - Compute the diplaced position.
    //
    displacedVertex = displaceVertexFunc(gl_Vertex, time*2.0, frequency );

    gl_Position = gl_ModelViewProjectionMatrix * displacedVertex;
    vVertex = gl_ModelViewMatrix * displacedVertex;


    // 2 - Compute the displaced normal
    //

    // if the engine does not provide the tangent vector you
    // can compute it with the following piece of of code:
    //
    vec3 tangent;
    vec3 binormal;

    vec3 c1 = cross(gl_Normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0));

    if(length(c1)>length(c2))
    {
        tangent = c1;
    }
    else
    {
        tangent = c2;
    }

    tangent = normalize(tangent);

    binormal = cross(gl_Normal, tangent);
    binormal = normalize(binormal);

    displacedNormal = computeNormal( displacedVertex.xyz,
                                     tangent.xyz,
                                     binormal,
                                     time*2.0,
                                     frequency );

    vNormal = normalize(gl_NormalMatrix * displacedNormal);
}
