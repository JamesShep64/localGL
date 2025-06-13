#pragma once
#include "Quad.h"
#include <vector>
#include <random>
class World{
public:
  World();
  NodeQuadManager manager;
  std::vector<uint> indices;
  void spawnNode(Node& n);
  void doSpawn();
private:
  float spawnRange = 20;
  float spawnDisplance = 50;
  int spawnIndex;
  void setVertices(Node& n);
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<float> dist;
};
