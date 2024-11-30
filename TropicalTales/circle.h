#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<time.h>
#include<stdlib.h>

#define CRES 30

class Circle
{
private:
	float x, y, radius, targetRadius, currentRadius, aspectRatio, ratio;
	float r, g, b, a;
	float growthDuration;
	float elapsedTime;
	bool isGrowing;
	
	GLuint vao, vbo;


public:
	Circle(float x, float y, float radius, float ratio, const float color [4]);
	~Circle();
	void render(GLuint shader);
	void clicked(float new_x, float new_y);
	void update(float deltaTime, float speed);

};

