// Autor: Aleksa Vukomanovic SV66/2021

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <chrono>
#include <thread>

#include "cloud.h"
#include "Island.h"
#include "water.h"
#include "celestial_body.h"
#include "campfire.h"
#include "moon.h"
#include "sun.h"
#include "palm_tree.h"
#include "circle.h"
#include "shark.h"
#include "smoke_letters_effect.h"

#include "text_renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int wWidth = 1920;
const unsigned int wHeight = 1080;
const float aspectRatio = (float) wWidth / wHeight;

const int TARGET_FPS = 60;
const float FRAME_TIME = 1.0f / TARGET_FPS; // 60 FPS target (1/60 seconds per frame)

bool clicked = false;

double clickX = 0.0f;
double clickY = 0.0f;

void updateSkyColor(float sunPositionY);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
void setLightColor(float sunPosition, GLuint shader);

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
    const char wTitle[] = "[Tropical Tales]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    //window = glfwCreateWindow(wWidth, wHeight, wTitle, glfwGetPrimaryMonitor(), NULL);

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "./res/Daydream.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    int textureWidth, textureHeight;
    
    unsigned char* imageData = stbi_load("./res/hooker_mouse.png", &textureWidth, &textureHeight, NULL, STBI_rgb_alpha);
    GLFWimage image;

    image.width = textureWidth;
    image.height = textureHeight;
    image.pixels = imageData;

    GLFWcursor* cursor = glfwCreateCursor(&image, 36, 36);
    glfwSetCursor(window, cursor);
   
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    stbi_image_free(imageData);
    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // Text renderer
    unsigned int textShader = createShader("text.vert", "text.frag");
    TextRenderer tr(face, ft, textShader, wWidth, wHeight);

    // Shader programs 
    unsigned int clickedCircleShader = createShader("circle_click.vert", "circle_click.frag");
    unsigned int shader = createShader("basic.vert", "basic.frag");
    unsigned int waterShader = createShader("water.vert", "basic.frag");
    unsigned int celestialShader = createShader("celestial.vert", "basic.frag");
    unsigned int campfireShader = createShader("campfire.vert", "campfire.frag");
    unsigned int starsShader = createShader("stars.vert", "stars.frag");
    unsigned int baseTextureShader = createShader("base_texture.vert", "base_texture.frag");
    unsigned int sharkShader = createShader("shark.vert", "base_texture.frag");
    
    // Objects used in this project

    GLuint fireTexture = loadImageToTexture("./res/campfire.png");
    GLuint palmTexture = loadImageToTexture("./res/palm_tree.png");
    GLuint sharkTexture = loadImageToTexture("./res/shark1.png");
    GLuint cloudTexture = loadImageToTexture("./res/cloud.png");
    GLuint firelightTexture = loadImageToTexture("./res/light.png");


    PalmTree palmTree1(0.5f, 0.1f, 0.2f, 0.3f, aspectRatio, palmTexture);
    PalmTree palmTree2(0.55f, 0.1f, 0.2f, 0.325f, aspectRatio, palmTexture);
    PalmTree palmTree3(0.65f, 0.1f, 0.2f, 0.325f, aspectRatio, palmTexture);
    PalmTree palmTree4(-0.7f, 0.15f, 0.2f, 0.325f, aspectRatio, palmTexture);
    
    Campfire campfire(0.4f, -0.05f, 0.1f, 0.1f, aspectRatio, fireTexture, firelightTexture);

    Island island(0.6f, -0.375f, 0.8f, 0.4f, (float) wWidth/wHeight, 0.8f, 0.6f, 0.4f);
    Island island1(-0.9f, -0.375, 0.425f, 0.475f, (float) wWidth/wHeight, 0.8f, 0.6f, 0.4f);

    Water water2(1.0f, 0.0f, 0.5f, 0.5f, 1.0f);
    Water water1(1.0f, -0.3375f, 0.5f, 0.5f, 1.0f);
    Sun sun(0.0f, -0.1f, (float) wWidth/wHeight, 0.05f, 0.575f, 0.8f, 0.8f, 0.2f);
    Moon moon(0.0f, -0.1f, (float) wWidth/wHeight, 0.05f, -0.575f, 0.8f, 0.8f, 0.8f);

    Shark shark(0.0f, -0.178f, 0.235f, 0.235f, sharkTexture);
    Shark shark1(0.2f, -0.01f, 0.225, 0.225f, sharkTexture);

    Cloud cloud1(0.0f, 0.3f, 0.25, 0.25, 0.05, cloudTexture);
    Cloud cloud2(0.3f, 0.4f, 0.25, 0.35, 0.15, cloudTexture);
    Cloud cloud3(-0.2f, 0.5f, 0.25, 0.35, 0.05, cloudTexture);
    Cloud cloud4(-0.5f, 0.6f, 0.25, 0.25, 0.25, cloudTexture);

    float circleColors[4] = { 1.0f, 0.0f, 0.0f, 0.5f };
    Circle clickedCircle(0.0f, 0.0f, 0.0f, aspectRatio, circleColors);
    

    SmokeLettersEffect sle(1300.0f, 600.0f);

    float timeSpeed = 1.0f;

    float previousTime = glfwGetTime();
    float deltaTime;
    float accumlatedTime = 0.0f;
    int previousBKeyState = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window))
    {
        // frame preparation
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        deltaTime = deltaTime * timeSpeed;
        accumlatedTime += deltaTime;

        // Input logic - extract in logic (ako stignem)
        int currentBKeyState = glfwGetKey(window, GLFW_KEY_B);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if (currentBKeyState == GLFW_RELEASE && previousBKeyState == GLFW_PRESS)
        {
            water1.switchRendering();
            water2.switchRendering();
        }
        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            std::cout << "Time reset" << std::endl;
            timeSpeed = 1.0f;
        }
        else if ((glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
            || (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS))
        {
            std::cout << "Slowing time down" << std::endl;
            if(timeSpeed > 0.1f) timeSpeed -= 0.05f;
        }
        else if ((glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS 
            && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) || (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS))
        {
            std::cout << "Speeding time up" << std::endl;
            if(timeSpeed < 2.0f) timeSpeed += 0.05f;
        }
        previousBKeyState = currentBKeyState;

        if (clicked) {
            clicked = false;
            if (water1.isClickedOn(clickY) || water2.isClickedOn(clickY)) {
                clickedCircle.clicked(clickX, clickY);
                shark.clicked(clickX);
                shark1.clicked(clickX);
            }
            if (campfire.isClickedOn(clickX, clickY)) {
                sle.clicked();
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        float waveHeight = sin(accumlatedTime) * timeSpeed * 0.001f;
        clickedCircle.update(deltaTime);
        shark.update(deltaTime, waveHeight);
        shark1.update(deltaTime, waveHeight);
        water1.update(waveHeight);
        water2.update(waveHeight);
        cloud1.update(deltaTime);
        cloud2.update(deltaTime);
        cloud3.update(deltaTime);
        cloud4.update(deltaTime);
        sun.update(deltaTime);
        moon.update(deltaTime);
        campfire.update(deltaTime);
        sle.updateSpawner(deltaTime);
        sle.updateLetters(deltaTime);

        float sunPosition = sun.getSunPosition();
        updateSkyColor(sunPosition);
        setLightColor(sunPosition, baseTextureShader);
        setLightColor(sunPosition, sharkShader);
        setLightColor(sunPosition, shader);
        setLightColor(sunPosition, waterShader);

        cloud1.render(sharkShader);
        cloud2.render(sharkShader);
        sun.render(celestialShader);
        moon.render(celestialShader, starsShader);
        cloud3.render(sharkShader);
        cloud4.render(sharkShader);
        shark1.render(sharkShader);
        water2.render(waterShader);
        island.render(shader);
        shark.render(sharkShader);
        island1.render(shader);
        water1.render(waterShader);
        palmTree1.render(baseTextureShader);
        palmTree2.render(baseTextureShader);
        palmTree3.render(baseTextureShader);
        palmTree4.render(baseTextureShader);

        campfire.render(campfireShader);

        tr.RenderText("Aleksa Vukomanovic SV66/2021", 10.0f, 50.0f, 0.8f, glm::vec3(0.8, 0.8f, 0.8f));
        sle.render(tr);

        clickedCircle.render(clickedCircleShader);
        float renderingTime = glfwGetTime() - currentTime;
        
        if (renderingTime < FRAME_TIME) {
            std::this_thread::sleep_for(std::chrono::duration<double>(FRAME_TIME - renderingTime));
        }
       
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glDeleteProgram(baseTextureShader);
    glDeleteProgram(campfireShader);
    glDeleteProgram(celestialShader);
    glDeleteProgram(starsShader);
    glDeleteProgram(waterShader);

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &clickX, &clickY);
        clickX = (clickX / wWidth) * 2.0f - 1.0f;
        clickY = 1.0f - (clickY / wHeight) * 2.0f;
        clicked = true;
        std::cout << "HERE: " << clickX << " | " << clickY << std::endl;
    }
}

void setLightColor(float sunPosition, GLuint shader) {
    float lightIntensity = sin(sunPosition * glm::pi<float>() / 2.0f);  
    glm::vec3 lightColor;
    if (lightIntensity > 0) {
        lightColor = glm::mix(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.6f, 0.5f, 0.3f), 1.0f - lightIntensity);
    } else {
        lightColor = glm::mix(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.3f, 0.3f, 0.3f), -lightIntensity);      
    }
    glUseProgram(shader);
    GLuint lightColorLoc = glGetUniformLocation(shader, "uLightColor");
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);
    glUseProgram(0);
}


void updateSkyColor(float sunYPos) {
    glm::vec3 skyColor;

    if (sunYPos > 0.75f) {
        skyColor = glm::mix(glm::vec3(0.53f, 0.81f, 0.98f), glm::vec3(0.8f, 0.9f, 1.0f), (sunYPos - 0.75f) / 0.25f);
    }
    else if (sunYPos > 0.05f) {
        float factor = (sunYPos - 0.25f) / 0.5f;
        skyColor = glm::mix(glm::vec3(1.0f, 0.6f, 0.2f), glm::vec3(0.53f, 0.81f, 0.98f), factor);
    }
    else if (sunYPos > -0.05f) {
        float factor = (sunYPos + 0.25f) / 0.5f;
        skyColor = glm::mix(glm::vec3(1.0f, 0.6f, 0.2f), glm::vec3(1.0f, 0.4f, 0.4f), factor);
    }
    else if (sunYPos < -0.75f) {
        float factor = (sunYPos + 0.75f) / 0.5f;
        skyColor = glm::mix(glm::vec3(0.2f, 0.1f, 0.3f), glm::vec3(0.05f, 0.05f, 0.1f), factor);
    }
    else {
        skyColor = glm::vec3(0.15f, 0.15f, 0.1f);
    }
    glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
}
