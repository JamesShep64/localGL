#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Vec3.h"

class Node{
public:
  static std::unordered_map<uint, std::unordered_map<uint, std::vector<uint>>> id_map;
  static std::vector<Node>* nodes;
  Node();
  Node(Vec3 pos, uint id);
  Node(float x, float y, float z, unsigned int id);
  Node(float x, float y, float z, unsigned int id, std::vector<uint>* connections);
  Vec3 pos;
  unsigned int id;
  bool surrounded = false;

  std::vector<Node*> connections;
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
  void addConnections(Node* a, Node* b);
  void connectToArea(std::vector<Node*> v);

  static bool checkWithin(Node* vertex, Node& point);
  static void setNodes(std::vector<Node>& n);
private:
static std::array<uint,3> sort_ids(uint a, uint b, uint c);
static bool check_triangle(uint a, uint b, uint c);
static void addTriangle(uint a, uint b, uint c);
static std::array<float, 2> vectorIntersection2D(Vec3 zero_t, Vec3 vec_t, Vec3 zero_u, Vec3 vec_u);

void getSurroundings(std::vector<Node*>& nodes,Node* node);
bool isHolding(std::vector<Node*>& nodes, Node* node);
};
