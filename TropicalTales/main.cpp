// Autor: Aleksa Vukomanovic SV66/2021


#define _CRT_SECURE_NO_WARNINGS

#define CRES 30


#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>  
#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>

#include "Island.h"
#include "water.h"
#include "celestial_body.h"
#include "campfire.h"
#include "moon.h"
#include "sun.h"
#include "palm_tree.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);

int main(void)
{

    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    const char wTitle[] = "[Generic Title]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    //window = glfwCreateWindow(wWidth, wHeight, wTitle, glfwGetPrimaryMonitor(), NULL);
    // glfwGetPrimaryMonitor()

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    int textureWidth, textureHeight;
    
    unsigned char* imageData = stbi_load("./res/hooker_mouse.png", &textureWidth, &textureHeight, NULL, STBI_rgb_alpha);
    GLFWimage image;

    image.width = textureWidth;
    image.height = textureHeight;
    image.pixels = imageData;

    GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
    glfwSetCursor(window, cursor);

    stbi_image_free(imageData);
    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }
    

    unsigned int shader = createShader("basic.vert", "basic.frag");
    unsigned int waterShader = createShader("water.vert", "basic.frag");
    unsigned int celestialShader = createShader("celestial.vert", "basic.frag");
    unsigned int campfireShader = createShader("campfire.vert", "campfire.frag");
    unsigned int starsShader = createShader("stars.vert", "stars.frag");
    unsigned int baseTextureShader = createShader("base_texture.vert", "base_texture.frag");

    float aspectRatio = (float) wWidth / wHeight;

    GLuint fireTexture = loadImageToTexture("./res/campfire.png");
    GLuint palmTexture = loadImageToTexture("./res/palm_tree.png");

    PalmTree palmTree1(0.5f, 0.1f, 0.2f, 0.3f, aspectRatio, palmTexture);
    PalmTree palmTree2(0.55f, 0.1f, 0.2f, 0.325f, aspectRatio, palmTexture);
    PalmTree palmTree3(0.65f, 0.1f, 0.2f, 0.325f, aspectRatio, palmTexture);
    PalmTree palmTree4(-0.7f, 0.15f, 0.2f, 0.325f, aspectRatio, palmTexture);
    
    Campfire campfire(0.4f, -0.05f, 0.1f, 0.1f, aspectRatio, fireTexture);

    Island island(0.6f, -0.375f, 0.8f, 0.4f, (float) wWidth/wHeight, 0.8f, 0.6f, 0.4f);
    Island island1(-0.9f, -0.375, 0.425f, 0.475f, (float) wWidth/wHeight, 0.8f, 0.6f, 0.4f);

    Water water2(2.0f, -0.0f, 0.5f, 0.5f, 1.0f);
    Water water1(2.0f, -0.675f, 0.5f, 0.5f, 1.0f);
    Sun sun(0.0f, -0.1f, (float) wWidth/wHeight, 0.05f, 0.575f, 0.8f, 0.8f, 0.2f);
    Moon moon(0.0f, -0.1f, (float) wWidth/wHeight, 0.05f, -0.575f, 0.8f, 0.8f, 0.8f);

    glClearColor(0.15, 0.15, 0.15, 1.0);

    float speed = 1.0f;
    double startTime = glfwGetTime();
    float now, end;
    float ms = 1.0f / 60.0f;

    while (!glfwWindowShouldClose(window))
    {
        now = glfwGetTime();

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {

        }
        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            std::cout << "Time reset" << std::endl;
        }
        else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        {
            std::cout << "Slowing time down" << std::endl;
        }
        else if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        {
            std::cout << "Speeding time up" << std::endl;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        sun.render(celestialShader, speed);
        moon.render(celestialShader, starsShader, speed);
        water2.render(waterShader, speed);
        island.render(shader);
        island1.render(shader);
           
        water1.render(waterShader, speed);
        palmTree1.render(baseTextureShader);
        palmTree2.render(baseTextureShader);
        palmTree3.render(baseTextureShader);
        palmTree4.render(baseTextureShader);

        campfire.render(campfireShader, speed);

        glfwSwapBuffers(window);

        end = glfwGetTime() - now;

        if (end < ms) {
            std::this_thread::sleep_for(std::chrono::duration<double>(ms - end));
        }

    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{

    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); 

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{


    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}


static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);

    if (ImageData != NULL) {
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(ImageData);
        return Texture;
    }
    else {
        std::cout << "Fail to load texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}
