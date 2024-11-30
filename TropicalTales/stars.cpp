#include "stars.h"


Stars::Stars() {

	float stars[STARS * 3];
	
	for (int i = 0; i < 100; i++) {
		float x = -1.0f + ((float) rand() / (RAND_MAX / 2.0f));
		float y = (float) rand() / RAND_MAX;
		float a = 0.5f + ((float) rand() / RAND_MAX) * 0.5f;
		stars[i * 3] = x;
		stars[i * 3 + 1] = y;
		stars[i * 3 + 2] = a;
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(stars), stars, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Stars::~Stars() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Stars::render(GLuint shader, float speed) {
	glUseProgram(shader);
	
    unsigned int uOpacityScale = glGetUniformLocation(shader, "opacityScale");
	glBindVertexArray(vao);
	glEnable(GL_BLEND);
	for (int i = 0; i < 10; i++) {
		float opacityScale = sin(((float)rand() / (RAND_MAX)) * speed);
		glPointSize((float)i);
		glUniform1f(uOpacityScale, opacityScale);
		glDrawArrays(GL_POINTS, i * 10, (i+1) * 10);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}
