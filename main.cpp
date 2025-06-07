#include <epoxy/gl.h>
#include "GraphicsEngine.h"
#include  <SDL2/SDL.h>
#include  <SDL2/SDL_opengl.h>
#include "Game.h"
#include "Material.h"
#include "glm/glm/ext/vector_float3.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>

GLuint VAO, VBO;
GLuint shaderProgram;
GraphicsEngine g;
Game game;
Material sky;
SDL_Window* pWindow;
bool quit = false;
std::vector<glm::vec3> triangleVertices = {
    glm::vec3(0.0f,  500.0f, 100.0f),
    glm::vec3(-500.0f, -500.0f, 100.0f),
     glm::vec3(500.0f, -500.0f, 100.0f)
};
std::vector<glm::vec3> verts = {
    glm::vec3(0.0f,  0.5f, 0.0f),
    glm::vec3(-0.5f, -0.5f, 0.0f),
    glm::vec3(0.5f, -0.5f, 0.0f)
};

void initGeometry(){
    g.init();
    game.init(pWindow);
    sky.loadObjToTriangleArray((char*)"./sphere.obj");
    if(!sky.loadTexture((char*)"./sky.jpg", g.skyShader,(char*)"sky"))
        std::cout<<"failed to load sky"<<"\n";
    shaderProgram = g.skyShader;
    }
void render(SDL_Window* window){
    int winWidth, winHeight;
    SDL_GetWindowSize(pWindow,&winWidth, &winHeight);
    glViewport(0,0,winWidth, winHeight);
    float z = 1.0;
    quit = game.update(z);

    glm::mat4 view = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 300.0f),  // camera further back
    glm::vec3(0.0f, 0.0f, 1.0f),  // looking at triangle
    glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f); // no transform
    glm::mat4 m = glm::scale(glm::mat4(1.0f),glm::vec3(3000,3000,3000));
    glm::mat4 perspective = glm::perspective(glm::radians(85.0f), 8.0f/6.0f, 0.1f, 1000.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Material::setTriangleBuffer(triangleVertices, g.shaders[g.landShader]);
    g.setCamera(g.landShader, game.cam);
    g.setModel(m,g.landShader);
    Material::drawTriangleArray(triangleVertices,g.shaders[g.landShader]);

    Material::setTriangleBuffer(sky.vertex_array, g.shaders[g.skyShader]);
    g.setCamera(g.skyShader, game.cam);
    g.setModel(m,g.skyShader);
    Material::drawTriangleArray(sky.vertex_array,g.shaders[g.skyShader]);
    
    SDL_GL_SwapWindow(pWindow);
}
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Set OpenGL attributes BEFORE creating window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // Try 2.1 instead of 2.0
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    // Create window with OpenGL flag
    pWindow = SDL_CreateWindow("Test Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!pWindow) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create OpenGL context
    SDL_GLContext glc = SDL_GL_CreateContext(pWindow);
    if (!glc) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    // Set swap interval AFTER context creation
    SDL_GL_SetSwapInterval(1);

    // Make context current
    if (SDL_GL_MakeCurrent(pWindow, glc) != 0) {
        fprintf(stderr, "SDL_GL_MakeCurrent failed: %s\n", SDL_GetError());
        SDL_GL_DeleteContext(glc);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    glViewport(0, 0, 800, 600);  
    initGeometry();
    // Compile shaders
        // Render loop
    int winWidth = 800, winHeight=600;
    bool running = true;
    while (running) {
        render(pWindow);
        SDL_GetWindowSize(pWindow, &winWidth, &winHeight);
        glViewport(0, 0, winWidth, winHeight);  
        if (quit){
            std::cout<<"QUIT QUIT QUIT QUIT QUIT QUIT QUIT"<<"\n";
            break;
        }
    }
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    return 0;
}
