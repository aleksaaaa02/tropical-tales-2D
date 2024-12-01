#version 330 core

out vec4 outCol;

uniform vec3 uCol;
uniform vec3 uLightColor;

void main()
{
	if (uLightColor == vec3(0.0, 0.0, 0.0)) {
		outCol = vec4(uCol, 1.0);
	}
	else {
		outCol = vec4(uCol, 1.0) * vec4(uLightColor, 1.0);
	}
}