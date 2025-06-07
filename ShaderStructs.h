#pragma once

#include <GL/gl.h>
struct Shader{
  GLint posAttrib;
  GLint texAttrib;
  GLint modelUni;
  GLint perspectiveUni;
  GLint viewUni;
  GLint timeColorUni;
  GLuint vbo;
  GLuint veo;
  GLuint vao;
  GLuint shaderProgram;
};
