#include "circle.h"


Circle::Circle(float x, float y, float radius, float ratio, const float color[4]) 
	: x(x), y(y), radius(radius), aspectRatio(ratio), isGrowing(false), ratio(ratio),
	currentRadius(radius), elapsedTime(0), growthDuration(0), targetRadius(radius + 0.1f){
	r = color[0];
	g = color[1];
	b = color[2];
	a = color[3];
	vao = 0;
	vbo = 0;
	
	float circle[2 * CRES + 4];
    circle[0] = x;
    circle[1] = y * ratio;
    for (int i = 0; i <= CRES; i++) {
        float angle = (3.141592 / 180) * (i * 360 / CRES);
        circle[2 + 2 * i] = (x + cos(angle) * r);
        circle[2 + 2 * i + 1] = (y + sin(angle) * r) * ratio;
    }
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Circle::~Circle() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Circle::render(GLuint shader) {
	if (!isGrowing) {
		return;
	}

	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
	trans = glm::scale(trans, glm::vec3(currentRadius, currentRadius, 0.0f));
	
	glUseProgram(shader);

	unsigned int uTransform = glGetUniformLocation(shader, "uTransform");
	unsigned int uColor = glGetUniformLocation(shader, "uCol");

	
	glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(trans));
	glUniform4f(uColor, r, g, b, a);

	glBindVertexArray(vao);
	glEnable(GL_BLEND);

	glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);

	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Circle::clicked(float new_x, float new_y) {
	if (isGrowing) {
		return;
	}
	this -> x = new_x;
	this -> y = new_y;
	this -> isGrowing = true;
	this -> growthDuration = 2.0f + ((float) rand() / RAND_MAX) * 3.0f;
	this -> currentRadius = radius;
	this -> elapsedTime = 0.0f;
}

void Circle::update(float deltaTime) {
	if (isGrowing) {

		elapsedTime += deltaTime;
		float t = (float) elapsedTime / growthDuration;
		currentRadius = (1.0f - t) * radius + t * targetRadius;

		if (elapsedTime >= growthDuration) {
			currentRadius = targetRadius;
			isGrowing = false;
		}
	}
}
