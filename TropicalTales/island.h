#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>

#define CRES 30

class Island
{
private:
	GLuint vao, vbo;
	float x, y;
	float rx, ry;
	const float r, g, b;

public:
	Island(float startX, float startY, float rx, float ry, float ratio, float r, float g, float b);
	~Island();
	void render(GLuint shader);
};
