#pragma once

#include <iostream>
#include <string>
#include <vector>

#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "text_renderer.h"

struct SmokeLetter {
	char character;
	float x, y;
	float lifetime;
	float opacity;
	float velocity;
 
	SmokeLetter(char c, float x, float y, float velY, float life)
        : character(c), x(x), y(y), velocity(velY), lifetime(life), opacity(1.0f) { }
};

class SmokeLettersEffect
{
private:

	float x, y;
	float letterSpawnTimer = 0.0f;
	float letterSpawnInterval = 0.5f;
	int currentLetterIndex = 0;
	bool spawningActive = false;

	std::string smokeMessage = "COMOP";
	std::vector<SmokeLetter> activeSmokeLetters;

public:
	SmokeLettersEffect(float, float);
	~SmokeLettersEffect();
	void render(TextRenderer&);
	void updateSpawner(float);
	void updateLetters(float);
	void clicked();
};

