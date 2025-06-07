#include "Game.h"
#include "glm/glm/ext/vector_float3.hpp"
#include "SDL_video.h"



Game::Game(){
}
void Game::init(SDL_Window* w){
    this->cam = Camera(glm::radians(85.0f), 800.0f/600.0f, .1f, 10000);
    this->input = Input(w);
}
bool Game::update(float){
    bool q = input.handleInputs();
    glm::vec3 camDelta = input.translateCam(cam.look);
    cam.translate(camDelta);
    glm::vec3 lookDelta = input.lookCam();
    cam.moveLook(lookDelta);
    input.windowInput();
    cam.setView();
    return q;
}
