#version 400 core

layout (points) in;
layout (line_strip, max_vertices = 128) out;

uniform int cbpp_CIRCLE_FIDELITY;
uniform float cbpp_CIRCLE_RADIUS;

uniform float cbpp_RATIO;

void main() {
    float PI = 3.141592;
    float step = 2*PI / cbpp_CIRCLE_FIDELITY;
    
    float ang = 0;
    vec2 p = vec2(0.0, 0.0);
    
    vec2 center = gl_in[0].gl_Position.xy;
    
    for(int i = 0; i < cbpp_CIRCLE_FIDELITY + 1; i++) {
        ang = i*step;
        p = vec2( cos(ang) / cbpp_RATIO, sin(ang) ) * cbpp_CIRCLE_RADIUS + center;
        gl_Position = vec4( p, 0.0, 1.0 );
        EmitVertex();
    }
    
    EndPrimitive();
}
