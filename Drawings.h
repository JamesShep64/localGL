#include <epoxy/gl.h>
#include "GraphicsEngine.h"
#include "Material.h"
#include "Vec3.h"
#include "glm/glm/ext/vector_float3.hpp"
#include <vector>
class Drawings{
public:
  static inline GLuint drawingShader;
  static inline GraphicsEngine g;
  enum Color{
      RED,
      GREEN,
      BLUE,
      WHITE,
      BLACK,
      YELLOW,
      CYAN,
      MAGENTA,
      ORANGE,
      PURPLE,
      GRAY,
      PINK,
      BROWN,
      COLOR_COUNT
  };
  static void init(GraphicsEngine& gr){g = gr;};
  static const glm::vec3 getColor(Color& c){
    static std::array<glm::vec3, COLOR_COUNT> color_values{
      glm::vec3(1.0f, 0.0f, 0.0f), // RED
      glm::vec3(0.0f, 1.0f, 0.0f), // GREEN
      glm::vec3(0.0f, 0.0f, 1.0f), // BLUE
      glm::vec3(1.0f, 1.0f, 1.0f), // WHITE
      glm::vec3(0.0f, 0.0f, 0.0f), // BLACK
      glm::vec3(1.0f, 1.0f, 0.0f), // YELLOW
      glm::vec3(0.0f, 1.0f, 1.0f), // CYAN
      glm::vec3(1.0f, 0.0f, 1.0f), // MAGENTA
      glm::vec3(1.0f, 0.5f, 0.0f), // ORANGE
      glm::vec3(0.5f, 0.0f, 0.5f), // PURPLE
      glm::vec3(0.5f, 0.5f, 0.5f), // GRAY
      glm::vec3(1.0f, 0.75f, 0.8f),// PINK
      glm::vec3(0.6f, 0.3f, 0.0f), // BROWN
    };
    return color_values[c];
  };
  static void setShader(GLuint shader){
    drawingShader = shader;
  };
  static const void drawSquare(Vec3& top, Vec3& bottom, Color c){
    top.print();
    bottom.print();
    std::vector<glm::vec3> square;
    //triangle one
    square.emplace_back(top.x,0,top.z);
    square.emplace_back(bottom.x,0,top.z);
    square.emplace_back(top.x,0,bottom.z);
    square.emplace_back(bottom.x,0,bottom.z);
    std::vector<uint> indices{
    0,1,2,
    3,1,2
    };
    g.setArrayToLand(drawingShader);
    g.setColor(getColor(c),drawingShader);
    Material::setTriangleBuffer(square,drawingShader);
    Material::setElementBuffer(indices, drawingShader);
    Material::drawElementArray(indices, drawingShader);
  }
};
