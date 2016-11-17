#version 330

in vec2 UV;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
    outColor = vec4(0, 1, 0, 0);
}
