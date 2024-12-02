#include "water.h"

Water::Water(float width, float height, float r, float g, float b)
    : width(width), height(height), r(r), g(g), b(b), x(0), y(0), renderWater(true) {
    
    float vertices[] = {
        -width, - 1.2f,
        width, - 1.2f,
        width, height,
        - width, height
    };
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Water::~Water() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Water::render(GLuint shader) {
    if (!renderWater) {
        return;
    }

    glUseProgram(shader);

    unsigned int uTransform = glGetUniformLocation(shader, "uTransform");
    unsigned int uColorLoc = glGetUniformLocation(shader, "uCol");

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0, y, 0.0f));
    
    glUniform3f(uColorLoc, r, g, b);
    glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(trans));
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);
}

void Water::update(float waveHeight) {
    y += waveHeight;
}

void Water::switchRendering() {
    renderWater = !renderWater;
}

bool Water::isClickedOn(float clickY) {
    return clickY < height;
}
