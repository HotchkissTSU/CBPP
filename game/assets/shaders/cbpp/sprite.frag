#version 400 core

uniform sampler2D Sampler;

in vec2 i_vUV;

void main() {
    gl_FragColor = texture(Sampler, i_vUV);
}
