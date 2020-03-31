#version 410 core

layout(quads, equal_spacing, ccw) in;

out vec2 TexCoord;

void main() {
    //texture coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    TexCoord = vec2(u, v);

    //25 control points
    vec4 p11 = gl_in[0].gl_Position;
    vec4 p12 = gl_in[1].gl_Position;
    vec4 p13 = gl_in[2].gl_Position;
    vec4 p14 = gl_in[3].gl_Position;
    vec4 p15 = gl_in[4].gl_Position;

    vec4 p21 = gl_in[5].gl_Position;
    vec4 p22 = gl_in[6].gl_Position;
    vec4 p23 = gl_in[7].gl_Position;
    vec4 p24 = gl_in[8].gl_Position;
    vec4 p25 = gl_in[9].gl_Position;

    vec4 p31 = gl_in[10].gl_Position;
    vec4 p32 = gl_in[11].gl_Position;
    vec4 p33 = gl_in[12].gl_Position;
    vec4 p34 = gl_in[13].gl_Position;
    vec4 p35 = gl_in[14].gl_Position;

    vec4 p41 = gl_in[15].gl_Position;
    vec4 p42 = gl_in[16].gl_Position;
    vec4 p43 = gl_in[17].gl_Position;
    vec4 p44 = gl_in[18].gl_Position;
    vec4 p45 = gl_in[19].gl_Position;

    vec4 p51 = gl_in[20].gl_Position;
    vec4 p52 = gl_in[21].gl_Position;
    vec4 p53 = gl_in[22].gl_Position;
    vec4 p54 = gl_in[23].gl_Position;
    vec4 p55 = gl_in[24].gl_Position;

    //compute the position 
    float u1 = (1-u) * (1-u) * (1-u) * (1-u) * (1-u);
    float u2 = 5 * u * (1-u) * (1-u) * (1-u) * (1-u);
    float u3 = 10 * u * u * (1-u) * (1-u) * (1-u);
    float u4 = 10 * u * u * u *  (1-u) * (1-u);
    float u5 = u * u * u * u * u;

    float v1 = (1-v) * (1-v) * (1-v) * (1-v) * (1-v);
    float v2 = 5 * v * (1-v) * (1-v) * (1-v) * (1-v);
    float v3 = 10 * v * v * (1-v) * (1-v) * (1-v);
    float v4 = 10 * v * v * v *  (1-v) * (1-v);
    float v5 = v * v * v * v * v;

    gl_Position = u1 * (v1*p11 + v2*p12 + v3*p13 + v4*p14 + v5*p15) +
        u2 * (v1*p21 + v2*p22 + v3*p23 + v4*p24 + v5*p25) +
        u3 * (v1*p31 + v2*p32 + v3*p33 + v4*p34 + v5*p35) +
        u4 * (v1*p41 + v2*p42 + v3*p43 + v4*p44 + v5*p45) +
        u5 * (v1*p51 + v2*p52 + v3*p53 + v4*p54 + v5*p55);
}

