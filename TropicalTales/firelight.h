#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Firelight
{

private:
	float cx, cy, a, b;
	float red, green, blue;
	GLuint vao, vbo, lightTexture;
	float scale;

public:
	Firelight(float cx, float cy, float a, float b, float ratio, GLuint texture);
	~Firelight();
	void update(float, float, float, float, float);
	void render(GLuint shader);
};

