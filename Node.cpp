#include "Node.h"

Node::Node(){

}
Node::Node(Sint x, Sint y){
    this->x = x;
    this->y = y;
}

Node Node::createNode(Node n){
    return Node(1,2);
}
