#pragma once
#include "Quad.h"
#include <vector>
class World{
public:
  World();
  NodeQuadManager manager;
  std::vector<uint> indices;
  void spawnNode(Node& n);
private:
  float spawnRange = 50;
  float spawnDisplance = 20;
  void setVertices(Node& n);
};
