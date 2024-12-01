#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

class Cloud
{
private:
    float cx, cy;
    float cloudSpeed;
    GLuint vao, vbo, texture;
public:
    Cloud(float cx, float cy, float a, float b, float cloudSpeed, GLuint cloudTexture);
    ~Cloud();
    void render(GLuint shader);
    void update(float deltaTime);

};

