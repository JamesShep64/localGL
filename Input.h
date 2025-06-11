#pragma once
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "glm/glm/ext/vector_float3.hpp"
#include <SDL2/SDL_video.h>
enum  keys{
  SPACE = 32,
  A=97,
  W=119,
  S=115,
  D=100,
  CTRL=1073742048,
  SHIFT= 1073742049,
  LEFT=1073741904,
  RIGHT=1073741903,
  UP=1073741906,
  DOWN=1073741905
};
class Input{
  public:
    Input(SDL_Window* w);
    Input();
    ~Input();
    bool wasPressed(keys key);
    bool isPressed(keys key);
    bool handleInputs();
    void windowInput();
   st
    glm::vec3 translateCam(glm::vec3 look);
    glm::vec3 lookCam();
  private:
    void handleKeyboard(SDL_Event& event);
    void handleMouse(SDL_Event& event);
    bool quit = false;
    std::map<int, bool> current_keyboard;
    std::map<int, bool> past_keyboard;
    Sint32 xCursorMotion = 0;
    Sint32 yCursorMotion = 0;
    SDL_Event event;
    SDL_Window* window;
    float yaw = 0;
    float pitch = 0;
    bool ignoreNextMouseInput = false;
};
