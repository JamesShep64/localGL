#pragma once
#include <epoxy/gl.h>
#include "../Camera.h"
#include "../ShaderStructs.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
class GraphicsEngine{
public:
  void init();
  
  void setModel(glm::mat4& model, GLuint& shaderProgram);
  void setView(glm::mat4& model, GLuint& shaderProgram);
  void setPerspective(glm::mat4& model, GLuint& shaderProgram);
  void setCamera(GLuint& shaderProgram, Camera& cam);

  void setArrayToSky(GLuint& shaderProgram);
  void setArrayToLand(GLuint& shaderProgram);
  GLuint skyShader;
  GLuint landShader;
  GLuint initShader(char* fragName, char* vertexName);
  std::map<GLuint, Shader> shaders;
private:
  static std::string LoadShaderSource(const std::string& filePath);
  void initSky(GLuint& shaderProgram);
  void initLand(GLuint& shaderProgram); void setShader(GLuint& shaderProgram);
  void debugShaderProgram(GLuint program);
  };
