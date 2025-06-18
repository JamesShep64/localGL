#pragma once
#include "glm/glm/ext/vector_float3.hpp"
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Vec3.h"
class Node{
struct Ray {Node* n;Vec3 zero; Vec3 vec;};
public:
  static std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>>& getId_map() {
    static std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> instance;
    return instance;
  }
  static std::vector<Node>* nodes;
  Node();
  Node(Vec3 pos, int id);
  Node(float x, float y, float z, unsigned int id);
  Node(float x, float y, float z, unsigned int id, std::vector<int>* connections);
  Vec3 pos;
  unsigned int id;
  bool surrounded = false;

  std::unordered_set<Node*> connections;
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
  bool connectToArea(std::vector<uint> v);

private:
  static std::array<int,3> sort_ids(const int& a,const int& b,const int& c);
  static bool check_triangle(int& a, int& b, int& c);
  static void addTriangle(const int& a,const int& b,const int& c);
  static std::array<float, 2> vectorIntersection2D(const Vec3& zero_t, const Vec3& vec_t,const Vec3& zero_u,const Vec3& vec_u);
  bool checkRayIntersection2D(const Vec3& point,const Ray& ray);

void getSurroundings(std::vector<Node*>& nodes,Node* node);
bool isHolding(std::vector<Node*>& nodes, Node* node);
};
