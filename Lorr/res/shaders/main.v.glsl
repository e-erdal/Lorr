#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;

out vec2 iUV;
out vec4 iColor;

uniform mat4 uTransform;

void main()
{
    iUV = aUV;
    iColor = aColor;
    
    gl_Position = uTransform * vec4(aPos, 1.0);
}