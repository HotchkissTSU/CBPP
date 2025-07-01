#version 400 core

uniform sampler2D Sampler;
uniform vec4 u_Color;

in vec2 t_vUV;

void main () {
    gl_FragColor = texture(Sampler, y_vUV) * u_Color;
}
