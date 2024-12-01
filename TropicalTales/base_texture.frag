#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 uLightColor;

void main()
{
	if (uLightColor == vec3(0.0, 0.0, 0.0)) {
		FragColor = texture(ourTexture, TexCoord);
	}
	else {
		FragColor = texture(ourTexture, TexCoord) * vec4(uLightColor, 1.0);
	}
}
