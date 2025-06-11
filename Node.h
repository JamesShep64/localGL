#pragma once

#include <array>
#include <memory>
#include <vector>
#include "Vec3.h"
class Node{
public:
  Node();
  Node(Vec3 pos, uint id);
  Node(float x, float y, float z, unsigned int id);
  Node(float x, float y, float z, unsigned int id, std::vector<uint>* connections);
  Vec3 pos;
  unsigned int id;
  bool surrounded = false;


  Node& operator=(const Node& other) {
        if (this != &other) {
            pos = other.pos;
            connections = other.connections;
            id = other.id;
        }
        return *this;
  };
  void setPosition(float newX, float newY, float newZ);
  float distanceTo(const Node& other);
  void print() const;
  void setLink();
  void setVert(Node& n);
  void checkSurrounded();
  Node* getClosestVertex(Node& n);

  static bool checkWithin(Node* vertex, Node& point);
private:

static std::array<float, 2> vectorIntersection2D(Vec3 zero_t, Vec3 vec_t, Vec3 zero_u, Vec3 vec_u);

void getSurroundings(std::vector<Node*>& nodes,Node* node);
bool isHolding(std::vector<Node*>& nodes, Node* node);

  std::vector<uint>* connections;
  Node* A = nullptr;
  Node* B = nullptr;
  Node* CloneB = nullptr;
  Node* CloneF = nullptr;
};
