#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 outColor;
out vec2 TexCoord;

uniform float scale;
const vec2 center = vec2(0.4, -0.05);
void main() {
	//gl_Position = vec4(aPos.x * scale, aPos.y * scale, 0.0, 1.0);
	vec2 scaledPos = (aPos - center) * scale + center;
    gl_Position = vec4(scaledPos, 0.0, 1.0);
	TexCoord = aTexCoord;

}
