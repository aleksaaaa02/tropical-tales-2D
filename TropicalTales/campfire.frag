#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 uCol;


void main()
{
	FragColor = texture(ourTexture, TexCoord) * uCol;
}