#pragma once
#include <epoxy/gl.h>
#include "../glm/glm/ext/vector_float2.hpp"
#include "../glm/glm/ext/vector_float3.hpp"
#include "../ShaderStructs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
struct Vertex{
    glm::vec3 position;
    glm::vec2 texCoordinate;
};
class Material{
public:
  Material();
  bool loadObjToTriangleArray(char* path);
  void loadElement(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);

template<typename T>
static void setTriangleBuffer(std::vector<T>& vertices,Shader s){
  glUseProgram(s.shaderProgram);
  glBindVertexArray(s.vao); 
  glBufferData(GL_ARRAY_BUFFER, 
                 vertices.size() * sizeof(T), 
                 vertices.data(), 
                 GL_STATIC_DRAW);
}

template<typename  T>
static void drawTriangleArray(std::vector<T>& vertices,Shader s){
  glUseProgram(s.shaderProgram);
  glBindVertexArray(s.vao); 
  glDrawArrays(GL_TRIANGLES,0,vertices.size());
}

template<typename T>
static void setElementBuffer(std::vector<T>& elements, GLuint shaderProgram){
  glUseProgram(shaderProgram);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 elements.size() * sizeof(T), 
                 &elements[0], 
                 GL_STATIC_DRAW);
}

  static void drawElementArray(std::vector<unsigned int>& elements, Shader s);

  bool loadTexture(char* path, GLuint shaderProgram, char* textureName);
  std::vector<Vertex> vertex_array;
  std::vector<GLuint> indices;
private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
};
