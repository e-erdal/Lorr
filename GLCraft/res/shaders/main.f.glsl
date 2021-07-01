#version 450

in vec2 iUV;
in vec4 iColor;

out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    FragColor = iColor;
}