#pragma once

class Node{
  typedef unsigned short int Sint;
public:
  Node();
  Node(Sint x, Sint y);
  Sint x;
  Sint y;
  static Node createNode(Node n);
private:
  Node* a;
  Node* b;
};
