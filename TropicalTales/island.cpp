#include "Island.h"


Island::Island(float startX, float startY, float rx, float ry, float r, float g, float b)
    : x(startX), y(startY), rx(rx), ry(ry), r(r), g(g), b(b){

    float circle[2 * CRES + 4];
    circle[0] = x;
    circle[1] = y;
    for (int i = 0; i <= CRES; i++) {
        float angle = (3.141592 / 180) * (i * 360 / CRES);
        circle[2 + 2 * i] = x + cos(angle) * rx;
        circle[2 + 2 * i + 1] = y + sin(angle)* ry;
    }
    
    unsigned int stride = 2 * sizeof(float);
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0); 
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Island::~Island() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Island::render(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    unsigned int uColLoc = glGetUniformLocation(shaderProgram, "uCol");
    glUniform3f(uColLoc, r, g, b);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);

    glBindVertexArray(0);
}
