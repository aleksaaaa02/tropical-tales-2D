#include "firelight.h"


Firelight::Firelight(float cx, float cy, float a, float b, float ratio, GLuint texture)
	:cx(cx), cy(cy), a(a + 0.1f), b(b + 0.1f), lightTexture(texture) {

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

Firelight::~Firelight() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Firelight::update(float deltaTime, float r, float g, float b, float scaleCampfire) {
	scale = scaleCampfire;
	red = r;
	green = g;
	blue = b;
}

void Firelight::render(GLuint shader) {

	glUseProgram(shader);
	glBindVertexArray(vao);
	unsigned int uTransform = glGetUniformLocation(shader, "uTransform");
	unsigned int uCol = glGetUniformLocation(shader, "uCol");

    glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, glm::vec3(cx, cy, 0.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1.0f));
	trans = glm::translate(trans, glm::vec3(-cx, -cy, 0.0f));

    glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(trans));
	glUniform4f(uCol, red, green, blue, 0.2f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);

}
