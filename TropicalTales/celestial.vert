#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;

out vec3 chCol;

uniform vec2 uPos;
uniform float rotationAngle;

void main() {

	gl_Position = vec4(inPos.x + uPos.x, inPos.y + uPos.y, 0.0, 1.0);
}
