#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<cmath>

class Water
{
private:
	float x, y;
	GLuint vao, vbo;
	float width, height;
	float r, g, b;
	bool renderWater;
public:
	Water(float width, float height, float r, float g, float b);
	~Water();
	void render(GLuint shader);
	void update(float waveHeight);
	void switchRendering();
};

