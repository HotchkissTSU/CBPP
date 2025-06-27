#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices=6) out;

uniform float cbpp_RATIO;
uniform float cbpp_SCALE;

out vec2 cbpp_vPos;
out vec2 cbpp_vUV;
out uint cbpp_iCharacter;

void main() {
    vec4 vTopLeft = gl_in[0].gl_Position;
        gl_Position = vTopLeft;
        EmitVertex();
        gl_Position = vTopLeft + vec4(cbpp_SCALE / cbpp_RATIO, 0.0, 0.0, 0.0);
        EmitVertex();
        gl_Position = vTopLeft + vec4(cbpp_SCALE / cbpp_RATIO, cbpp_SCALE, 0.0, 0.0);
        EmitVertex();
    EndPrimitive();
        gl_Position = vTopLeft;
        EmitVertex();
        gl_Position = vTopLeft + vec4(0.0, cbpp_SCALE, 0.0, 0.0);
        EmitVertex();
        gl_Position = vTopLeft + vec4(cbpp_SCALE / cbpp_RATIO, cbpp_SCALE, 0.0, 0.0);
        EmitVertex();
    EndPrimitive();
}
