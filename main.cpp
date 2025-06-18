#include <epoxy/gl.h>
#include "GraphicsEngine.h"
#include  <SDL2/SDL.h>
#include  <SDL2/SDL_opengl.h>
#include "Game.h"
#include "Material.h"
#include "Drawings.h"
#include "World.h"
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
    glm::vec3(0.0f,  0.0f, 100.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(100.0f, 0.0f, 0.0f)
};
std::vector<GLuint> elements = {
    0,1,2
};

void initGeometry(){
    g.init();
    Drawings::init(g);
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

    glm::mat4 model = glm::mat4(1.0f); // no transform
    glm::mat4 m = glm::scale(glm::mat4(1.0f),glm::vec3(3000,3000,3000));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//draw land
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    g.setArrayToLand(g.landShader);
    Material::setTriangleBuffer(game.world.manager.getVertices(), g.landShader);
    g.setCamera(g.landShader, game.cam);
    g.setModel(model,g.landShader);
    g.setColor(glm::vec3(1.0f, 0.0f, .937f), g.landShader);
    Material::setElementBuffer(game.world.indices,g.landShader);
    Material::drawElementArray(game.world.indices,g.landShader);
//draw spawn
    Drawings::setShader(g.landShader);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    g.setArrayToLand(g.landShader);
    g.setCamera(g.landShader, game.cam);
    g.setModel(model,g.landShader);
    Drawings::drawSquare(game.world.topSpawnCorner,game.world.bottomSpawnCorner,Drawings::YELLOW);
//draw sky
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    g.setArrayToSky(g.skyShader);
    Material::setTriangleBuffer(sky.vertex_array, g.skyShader);
    g.setCamera(g.skyShader, game.cam);
    g.setModel(m,g.skyShader);
    Material::drawTriangleArray(sky.vertex_array,g.skyShader);
    
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
