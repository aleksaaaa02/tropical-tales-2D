#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<cmath>

class Water
{
private:
	GLuint vao, vbo;
	float width, height;
	float r, g, b;
public:
	Water(float width, float height, float r, float g, float b);
	~Water();
	void render(GLuint shader);
};

