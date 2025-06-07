#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/gl.h>
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
class Camera{
public:
  Camera();
  Camera(GLfloat fov,GLfloat aspect,GLfloat  near,GLfloat  far);
  void setView();
  void translate(glm::vec3 t);
  void moveLook(glm::vec3 t);
  glm::mat4 getView();
  glm::mat4 perspective;
  glm::mat4 view;
  glm::vec3 look;
private:
  glm::vec3 position;
};
