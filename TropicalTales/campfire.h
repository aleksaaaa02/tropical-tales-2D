#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class Campfire
{
private:
	GLuint vao, vbo, texture;
	float x, y, a, b;
	float ratio;

public:
	Campfire(float cx, float cy, float a, float b, float ratio, GLuint texture);
	~Campfire();
	void render(GLuint shader);
};

