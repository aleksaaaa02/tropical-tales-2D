#include "cloud.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cloud::Cloud(float cx, float cy, float a, float b, float cloudSpeed, GLuint cloudTexture)
	: cx(cx), cy(cy), cloudSpeed(cloudSpeed), texture(cloudTexture) 
{

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

Cloud::~Cloud() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


void Cloud::update(float deltaTime) {
    cx += deltaTime * cloudSpeed;

    if (cx > 1.5f) {  
        cx = -1.5f;
    }
}

void Cloud::render(GLuint shader) {

    glUseProgram(shader);
   
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(cx, 0.0f, 0.0f));
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
