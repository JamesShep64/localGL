#pragma once
#include "Quad.h"
#include "glm/glm/ext/vector_float3.hpp"
#include <vector>
#include <random>


class World{
public:
  World();
  NodeQuadManager manager;
  std::vector<uint> indices;
  void spawnNode(Node& n);
  void doSpawn();
  Vec3 topSpawnCorner = Vec3(0,0,0);
  Vec3 bottomSpawnCorner = Vec3(0,0,0);
private:
  float spawnRange = 20;
  float spawnDisplance = 50;
  int spawnIndex;
  void setVertices();
  void drawBox();
  void drawLine();
  void drawPoint();
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<float> dist;
};
