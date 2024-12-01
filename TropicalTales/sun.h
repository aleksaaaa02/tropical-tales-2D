#pragma once

#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CRES 30

class Sun
{
private:
	GLuint vao, vbo;
	float x, y;
	float radius, cycleRadius, ratio;
	float r, g, b;
	float elapsedTime;

public:
	Sun(float x, float y, float ratio, float radius, float cycleRadius, float r, float g, float b);
	~Sun();
	void render(GLuint shader);
	void update(float deltaTime);
	float getSunPosition();
};

