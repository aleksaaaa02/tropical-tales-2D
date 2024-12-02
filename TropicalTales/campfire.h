#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "firelight.h"

class Campfire
{
private:
	GLuint vao, vbo, texture;
	float x, y, a, b;
	float red, green, blue;
	float ratio;
	float scale;
	float elapsedTime;
	Firelight firelight;

public:
	Campfire(float cx, float cy, float a, float b, float ratio, GLuint texture, GLuint lightTexture);
	~Campfire();
	void render(GLuint shader);
	void update(float deltaTime);
	void interact();
	bool isClickedOn(float, float);
};

