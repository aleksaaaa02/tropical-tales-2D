#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;

uniform float waveHeight; 

out vec3 fragColor;

void main()
{
    gl_Position = vec4(inPos.x, inPos.y + waveHeight, 0.0, 1.0);
    fragColor = inCol;
}