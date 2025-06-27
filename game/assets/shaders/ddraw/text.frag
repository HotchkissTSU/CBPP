#version 400 core

in vec2 cbpp_vUV;

uniform sampler2D samp;

void main() {
    gl_FragColor = texture(samp, cbpp_vUV);
}

