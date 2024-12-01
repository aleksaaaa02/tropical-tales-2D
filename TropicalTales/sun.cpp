#include "sun.h"



Sun::Sun(float x, float y, float ratio, float radius, float cycleRadius, float r, float g, float b) 
	: x(x), y(y), radius(radius), cycleRadius(cycleRadius), ratio(ratio), r(r), g(g), b(b), elapsedTime(0) {

	float circle[2 * CRES + 4];

    circle[0] = x;
    circle[1] = y * ratio;
    std::cout << ratio;
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

Sun::~Sun() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Sun::render(GLuint shader) {
	glUseProgram(shader);
	
    unsigned int uColLoc = glGetUniformLocation(shader, "uCol");
    glUniform3f(uColLoc, r, g, b);

    unsigned int uTransform = glGetUniformLocation(shader, "uTransform");

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
    glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(trans));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CRES + 2);

    glBindVertexArray(0);
	glUseProgram(0);
}

void Sun::update(float deltaTime) {
    elapsedTime += deltaTime * 0.2f;
    float xPos = cos(elapsedTime) * cycleRadius;
    float yPos = sin(elapsedTime) * cycleRadius * ratio;
    x = xPos;
    y = yPos;
}
float Sun::getSunPosition() {
	return y;
}
