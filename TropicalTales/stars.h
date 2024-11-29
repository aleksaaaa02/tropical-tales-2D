#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<time.h>
#include<stdlib.h>

#define STARS 100

class Stars
{
private:
	GLuint vao, vbo;

public:
	Stars();
	~Stars();
	void render(GLuint shader, float speed);
};

