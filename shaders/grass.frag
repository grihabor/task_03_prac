#version 330

in vec2 UV;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
    outColor = texture(textureSampler, UV).rgba;
    if(outColor.a < 1.)
        discard;

}
