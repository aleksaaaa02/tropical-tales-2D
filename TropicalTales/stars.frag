#version 330 core

in float alpha;

out vec4 outCol;

uniform vec3 uCol;
uniform float opacityScale;

void main() 
{
	outCol = vec4(1.0f, 1.0f, 1.0f, opacityScale * alpha);
}