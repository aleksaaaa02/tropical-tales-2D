#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#define CRES 30

class CelestialBody
{
private:
	GLuint vao, vbo;
	float x, y;
	float radius, cycleRadius;
	float speed;
	float r, g, b;

public:
	CelestialBody(float x, float y, float ratio, float radius, float cycleRadius, float speed, float r, float g, float b);
	~CelestialBody();
	void render(GLuint shader);
};

