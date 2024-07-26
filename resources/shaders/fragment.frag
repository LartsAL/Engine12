#version 330 core

out vec4 color;

in vec2 textureCoord;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

uniform float mixKoef;

void main() {
    color = mix(texture(myTexture1, textureCoord), texture(myTexture2, textureCoord), mixKoef);
}