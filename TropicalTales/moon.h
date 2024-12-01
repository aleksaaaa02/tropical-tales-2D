#pragma once

#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
#include "stars.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CRES 30

class Moon
{
private:
	Stars stars;
	GLuint vao, vbo;
	float x, y;
	float radius, cycleRadius, ratio;
	float r, g, b;
	float elapsedTime;

public:
	Moon(float x, float y, float ratio, float radius, float cycleRadius, float r, float g, float b);
	~Moon();
	void render(GLuint shader, GLuint starsShader);
	void update(float deltaTime);
};

