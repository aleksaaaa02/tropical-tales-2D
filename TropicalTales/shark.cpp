#include "shark.h"

void handleTargeMovment() {

}

void handleLoopMovment() {

}

Shark::Shark(float cx, float cy, float a, float b, GLuint texture) 
    : x(cx), y(cy), texture(texture), moveRight(true), targetX(0), hasTarget(false) {
	float vertices[] = {
		cx - a / 2, cy - b / 2, 0.0f, 0.0f,
		cx + a / 2, cy - b / 2, 1.0f, 0.0f,
		cx - a / 2, cy + b / 2, 0.0f, 1.0f,
		cx + a / 2, cy + b / 2, 1.0f, 1.0f,
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

Shark::~Shark() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Shark::update(float deltaTime, float speed, float waveHeight) {
	// move shark to the right or to do left depending on moveRight condition
	// we can add if clicked to translate it to go to Y position 
    if (hasTarget) {
        float dx = targetX - x;
        if (std::abs(dx) < 0.1f) {
            hasTarget = false;
            x = targetX;
        }
        else {
            if (dx > 0) {
                x += deltaTime * speed * 0.7;
                if (x > targetX) x = targetX;
                moveRight = true;
            }
            else {
                x -= deltaTime * speed * 0.7;
                if (x < targetX) x = targetX;
                moveRight = false;
            }
        }
    }
    else {
	    if (moveRight) {
            x += speed * deltaTime * 0.4;
            if (x > 1.0f) moveRight = false;
        }   
        else {
            x -= speed * deltaTime * 0.4;
            if (x < -1.0f) moveRight = true;
        }
    }

    y += waveHeight;

}

void Shark::render(GLuint shader) {
    glUseProgram(shader);
   

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
    if (!moveRight) {
        transform = glm::scale(transform, glm::vec3(-1.0f, 1.0f, 1.0f));
    }
    GLint transformLoc = glGetUniformLocation(shader, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glActiveTexture(GL_TEXTURE0);

    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Shark::clicked(float clickedX) {

    if (hasTarget) {
        return;
    }
    hasTarget = true;
    targetX = clickedX;

}

