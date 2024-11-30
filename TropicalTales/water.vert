#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;

uniform float waveHeight; 
uniform mat4 uTransform;

out vec3 fragColor;

void main()
{
    gl_Position = uTransform * vec4(inPos, 0.0, 1.0);
    fragColor = inCol;
}