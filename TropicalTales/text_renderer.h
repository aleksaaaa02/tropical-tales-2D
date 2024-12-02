#pragma once
#include <iostream>
#include <map>
#include <string>

#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;
    glm::ivec2   Size;      
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class TextRenderer
{
private:
    std::map<GLchar, Character> characters;
    GLuint vao, vbo;
    GLuint shader;

public:

    TextRenderer(FT_Face, FT_Library, GLuint, unsigned int, unsigned int);
    ~TextRenderer();
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

};

