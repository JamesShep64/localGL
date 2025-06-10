#include <cstdio>
#include <iostream>
#include <cmath>
#include "Input.h"
#include <SDL2/SDL_events.h>
#include "glm/glm/common.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/geometric.hpp"
#include "glm/glm/trigonometric.hpp"
#include <string>

Input::Input(){
}
Input::Input(SDL_Window* w){
  window = w;
}
Input::~Input(){

}
bool Input::handleInputs(){
  /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
  /* more events on the event queue, our while loop will exit when */
  /* that occurs.                                                  */
  //gnoreNextMouseInput = true;
  while( SDL_PollEvent( &event ) ){
    if(event.type == SDL_QUIT)
      quit = true;
    this->handleMouse(event);
    this->handleKeyboard(event);
  }
  return quit;
}
void Input::handleMouse(SDL_Event& event){
    switch( event.type ){
        case SDL_MOUSEMOTION:
          //xCursorMotion = event.motion.x;
          //yCursorMotion = event.motion.y;
        break;
        default:
        break;
    }
}
void Input::handleKeyboard(SDL_Event& event){
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    past_keyboard = current_keyboard;
    switch( event.type ){
      case SDL_KEYDOWN:
        //std::cout<<"DOWN "<<std::to_string(event.key.keysym.sym)<<"\n";
        current_keyboard[event.key.keysym.sym] = true;
        break;

      case SDL_KEYUP:
        //std::cout<<"UP "<<event.key.keysym.sym<<"\n";
        current_keyboard[event.key.keysym.sym] = false;
        break;

      default:
        break;
    }
}
void Input::windowInput(){
      //SDL_SetRelativeMouseMode(SDL_TRUE);
}
glm::vec3 Input::translateCam(glm::vec3 look){
    yCursorMotion=0;
    xCursorMotion=0;
    glm::vec3 delta(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0,1,0);
    glm::vec3 forward = look;
    glm::vec3 left = glm::normalize(glm::cross(up, forward)) * 4.0f;
    if(this->isPressed(A))
      delta+=left;
    if(this->isPressed(D))
      delta-=left;
    if(this->isPressed(S))
      delta-=forward; 
    if(this->isPressed(W))
      delta+=forward; 
    if(this->isPressed(SPACE))
      delta+=up; 
    if(this->isPressed(SHIFT))
      delta-=up; 
    if(this->isPressed(LEFT))
      xCursorMotion-=15;
    if(this->isPressed(RIGHT))
      xCursorMotion+=15;
    if(this->isPressed(UP))
      yCursorMotion-=15;
    if(this->isPressed(DOWN))
      yCursorMotion+=15;
    return delta;
}
glm::vec3 Input::lookCam(){
    if(xCursorMotion == 0 && yCursorMotion == 0)
      return glm::vec3(0,0,0);
    yaw += float(xCursorMotion) * .1f;
    pitch -= float(yCursorMotion) * .1f;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    glm::vec3 delta(std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),std::sin(glm::radians(pitch)), std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch)));
    xCursorMotion = 0;
    yCursorMotion = 0;
    return glm::normalize(delta);
}
bool Input::wasPressed(keys key){
  if(!past_keyboard[key] && current_keyboard[key]){
    return true;
  }
  return false;
}
bool Input::isPressed(keys key){
  return current_keyboard[key];
}
