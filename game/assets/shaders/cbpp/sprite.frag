#version 400 core

uniform sampler2D Sampler;
uniform vec4 cbpp_Color;

in vec2 vUV_flipped;

void main() {
    gl_FragColor = texture(Sampler, vUV_flipped) * cbpp_Color;
}
