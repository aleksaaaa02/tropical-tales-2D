#include "water.h"

Water::Water(float width, float height, float r, float g, float b)
    : width(width), height(height), r(r), g(g), b(b) {
    
    float vertices[] = {
        -width / 2, - 1.2f,
        width / 2, - 1.2f,
        width / 2, height / 2,
        - width / 2, height / 2
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
    glUseProgram(shader);

    unsigned int uWaveHeightLoc = glGetUniformLocation(shader, "waveHeight");
    unsigned int uColorLoc = glGetUniformLocation(shader, "uCol");

    glUniform3f(uColorLoc, r, g, b);

    float time = glfwGetTime();  
    float waveHeight = sin(time) * 0.05f;

    glUniform1f(uWaveHeightLoc, waveHeight);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);
}
