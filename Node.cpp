#include "Node.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

Node::Node(){

}
Node::Node(float x, float y, float z,unsigned int id){
    pos.x = x;
    pos.y = y;
    pos.z = z;
    this->id = id;
}
void Node::setPosition(float newX, float newY, float newZ) {
        pos.x = newX;
        pos.y = newY;
        pos.z = newZ;
}
float Node::distanceTo(const Node& other){
        float dx = pos.x - other.pos.x;
        float dy = pos.y - other.pos.y;
        float dz = pos.z - other.pos.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
}
void Node::setVert(Node& n){
    if(A == nullptr)
        A = &n;
    else if(A == nullptr)
        B = &n;
 if (std::find(connections->begin(), connections->end(), n.id) != connections->end())
        std::cerr<<"CONNECTING TO SAME NODE TWICE\n";
    connections->push_back(n.id);
}

bool Node::checkWithin(Node* vertex, Node& point){
    if(vertex == nullptr)
        return false;
    if(vertex->A == nullptr || vertex->B == nullptr){
        std::cerr<<"CheckWithin with unlinked node\n";
        return false;
    }
    auto t_u = Node::vectorIntersection2D(vertex->pos,vertex->pos-point.pos, vertex->A->pos, vertex->B->pos);
    if(t_u[0] > 1 && t_u[1] > 0 && t_u[1] < 1)
        return true;
    return checkWithin(vertex->CloneF, point) || checkWithin(vertex->CloneB, point);
}

std::array<float, 2> Node::vectorIntersection2D(Vec3 zero_t, Vec3 vec_t, Vec3 zero_u, Vec3 vec_u){
    float h = vec_t.x * vec_u.y - vec_t.y * vec_u.x;
    float t = (zero_u.x - zero_t.x) * vec_u.y - (zero_u.y - zero_t.y) * vec_u.x;
    t/=h;
    
    float u = (zero_u.x - zero_t.x) * vec_t.y - (zero_u.y - zero_t.y) * vec_t.x;
    u/=h;
    return std::array<float,2>{t,u};
}

void Node::checkSurrounded(){
    std::vector<Node*> nodes;
    getSurroundings(nodes, this);
    for(Node* node : nodes){
        if(!this->isHolding(nodes, node)){
            return;
        }
    }
    this->surrounded = true;
}
void Node::getSurroundings(std::vector<Node*>& nodes,Node* node){
    if(node == nullptr)
        return;
    if (std::find(node->connections->begin(), node->connections->end(), this->id) == connections->end())
        return;
    if (std::find(nodes.begin(), nodes.end(), node) != nodes.end())
        return;
    nodes.push_back(node);
    getSurroundings(nodes, node->A);
    getSurroundings(nodes, node->B);
    getSurroundings(nodes, node->CloneF);
    getSurroundings(nodes, node->CloneB);
}

bool Node::isHolding(std::vector<Node*>& nodes, Node* node){
    if(node == nullptr)
        return false;
    for(uint c_id : *node->connections){
        if (std::find(node->connections->begin(), node->connections->end(), c_id) == connections->end())
            return true;
    }
    return isHolding(nodes,node->CloneF) || isHolding(nodes,node->CloneB);
}
void Node::print() const{
    std::cout << "Node(id=" << id << ", x=" << pos.x << ", y=" <<  pos.y<< ", z=" <<  pos.z << ")" << std::endl;
}


