#pragma once

#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>

#define CRES 30

class Sun
{
private:
	GLuint vao, vbo;
	float x, y;
	float radius, cycleRadius, ratio;
	float r, g, b;

public:
	Sun(float x, float y, float ratio, float radius, float cycleRadius, float r, float g, float b);
	~Sun();
	void render(GLuint shader, float speed);
};

