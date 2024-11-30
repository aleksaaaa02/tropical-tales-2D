#include "celestial_body.h"

CelestialBody::CelestialBody(float x, float y, float ratio, float radius, float cycleRadius, float r, float g, float b) 
	: x(x), y(y), radius(radius), cycleRadius(cycleRadius), ratio(ratio), r(r), g(g), b(b) {

	float circle[2 * CRES + 4];

    circle[0] = x;
    circle[1] = y * ratio;
    for (int i = 0; i <= CRES; i++) {
        float angle = (3.141592 / 180) * (i * 360 / CRES);
        circle[2 + 2 * i] = (x + cos(angle) * radius);
        circle[2 + 2 * i + 1] = (y + sin(angle) * radius) * ratio;
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

CelestialBody::~CelestialBody() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void CelestialBody::render(GLuint shader, float speed) {
	glUseProgram(shader);
	
    unsigned int uColLoc = glGetUniformLocation(shader, "uCol");
    unsigned int uPos = glGetUniformLocation(shader, "uPos");
    glUniform3f(uColLoc, r, g, b);
    
    float angle = glfwGetTime() * speed * 0.5;
    float xPos = cos(angle) * cycleRadius;
    float yPos = sin(angle) * cycleRadius * ratio;
    glUniform2f(uPos, xPos, yPos);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);

	glUseProgram(0);
}

