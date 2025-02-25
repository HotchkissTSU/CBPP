#version 400 core

uniform sampler2D samp;

in vec2 uv;

void main() {
    gl_FragColor = texture(samp, uv);
}
