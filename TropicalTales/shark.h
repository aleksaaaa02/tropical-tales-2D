#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shark
{

private:
	float x, y;
	GLuint vao, vbo, texture;
	bool moveRight;
	bool hasTarget;
	float targetX;

public:
	Shark(float cx, float cy, float a, float b, GLuint texture);
	~Shark();
	void update(float deltaTime, float speed, float waveHeight);
	void render(GLuint shader);
	void clicked(float clickedX);

};

