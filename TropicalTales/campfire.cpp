#include "campfire.h"


Campfire::Campfire(float cx, float cy, float a, float b, float ratio, GLuint texture, GLuint lightTexture) 
	: x(cx), y(cy), texture(texture), ratio(ratio), a(a), b(b), elapsedTime(0), red(1.0f), green(0.5f), blue(0.0f)
	, firelight(cx, cy, a, b, ratio, lightTexture) {

	float vertices[] = {
		cx - a / 2, (cy - b / 2) * ratio, 0.0f, 0.0f,
		cx + a / 2, (cy - b / 2) * ratio, 1.0f, 0.0f,
		cx - a / 2, (cy + b / 2) * ratio, 0.0f, 1.0f,
		cx + a / 2, (cy + b / 2) * ratio, 1.0f, 1.0f,
	};

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindTexture(GL_TEXTURE_2D, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Campfire::~Campfire() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Campfire::render(GLuint shader) {

	glUseProgram(shader);
	glBindVertexArray(vao);

	unsigned int uTransform = glGetUniformLocation(shader, "uTransform");
	unsigned int uCol = glGetUniformLocation(shader, "uCol");

    glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1.0f));
	trans = glm::translate(trans, glm::vec3(-x, -y, 0.0f));

    glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(trans));
	glUniform4f(uCol, red, green, blue, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	firelight.render(shader);

}
void Campfire::update(float deltaTime) {
	elapsedTime += deltaTime;

	scale = 1.0f + 0.2f * sin(elapsedTime * 0.5f);
	red = 1.0f;
	green = 0.5f + 0.3f * sin(elapsedTime * 3.0f);
	blue = 0.0f;

	firelight.update(deltaTime, red, green, blue, scale + 0.1);
}

void Campfire::interact() {

}

bool Campfire::isClickedOn(float clickX, float clickY) {
	float dx = std::abs(clickX - x);
	float dy = std::abs(clickY - y);
	return dx < 0.1 && dy < 0.1;
}
