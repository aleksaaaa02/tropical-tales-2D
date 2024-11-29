#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class PalmTree
{
private:
	GLuint vao, vbo, texture;
	float x, y, a, b;
	float ratio;

public:
	PalmTree(float cx, float cy, float a, float b, float ratio, GLuint texture);
	~PalmTree();
	void render(GLuint shader);

};

