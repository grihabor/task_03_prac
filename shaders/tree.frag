#version 330

in vec2 UV;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
    vec3 color = texture(textureSampler, UV).rgb;
    outColor = vec4(color.r * 1.1, color.g * 1.1, color.b * .7, 1.);
}
