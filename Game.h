#pragma once
#include "Camera.h"
#include "Input.h"
#include "SDL_video.h"
class Game{
  public:
    Game();
    void init(SDL_Window* w);
    bool update(float z);
    Camera cam;
    Input input;
  private:
};
