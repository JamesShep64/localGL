#include "Node.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>



std::unordered_map<uint, std::unordered_map<uint, std::vector<uint>>> Node::id_map;
std::vector<Node>* nodes = nullptr;
Node::Node(Vec3 pos, uint id){
    this->pos = pos;
    this->id = id;
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
void Node::addConnections(Node* a, Node* b){
    connections.push_back(a);
    connections.push_back(b);
}

void Node::connectToArea(std::vector<uint> n){

    if(!(t_u[1] < 1 && t_u[1] > 0 && t_u[0] < 1 && t_u[0] > 0))
        return p[0];
    return B;
    }
}

bool Node::checkWithin(Node* vertex, Node& point){
    if(vertex == nullptr)
        return false;
    if(vertex->A == nullptr || vertex->B == nullptr){
        std::cerr<<"CheckWithin with unlinked node\n";
        return false;
    }
    auto t_u = Node::vectorIntersection2D(vertex->pos, point.pos - vertex->pos, vertex->A->pos, vertex->B->pos - vertex->A->pos);
    std::cout<<"t = "<<t_u[0]<<" u = "<<t_u[1]<<"\n";
    if(t_u[0] >= 1 && t_u[1] >= 0 && t_u[1] <= 1)
        return true;
    return checkWithin(vertex->CloneF, point) || checkWithin(vertex->CloneB, point);
}

std::array<float, 2> Node::vectorIntersection2D(Vec3 zero_t, Vec3 vec_t, Vec3 zero_u, Vec3 vec_u){
    float h = vec_t.x * vec_u.z - vec_t.z * vec_u.x;
    float t = (zero_u.x - zero_t.x) * vec_u.z - (zero_u.z - zero_t.z) * vec_u.x;
    t/=h;
    
    float u = (zero_u.x - zero_t.x) * vec_t.z - (zero_u.z - zero_t.z) * vec_t.x;
    u/=h;
    return std::array<float,2>{t,u};
}

std::array<uint,3> Node::sort_ids(uint a, uint b, uint c){
    uint smallest = std::min({a, b,c});
    uint largest  = std::max({a, b,c});
    uint middle   = a+ b+ c - smallest - largest;
    return {smallest, middle, largest};
}

bool Node::check_triangle(uint a, uint b, uint c){
    auto sorted = sort_ids(a,b,c);
    auto outer_it = id_map.find(sorted[0]);
    if (outer_it != id_map.end()) {
        auto inner_it = outer_it->second.find(sorted[1]);
        if (inner_it != outer_it->second.end()) {
            const auto& vec = inner_it->second;
            return std::find(vec.begin(), vec.end(), sorted[2]) != vec.end();
        }
    }
    return false;
}

void Node::addTriangle(uint a, uint b, uint c){
    if(a == b || a == c || b ==c){
        std::cerr<<"ADDING TRI WITH REPEAT NODE\n";
        return;
    }
    auto sorted = sort_ids(a, b, c);
    id_map[sorted[0]][sorted[1]].push_back(sorted[2]);
}

void setNodes(std::vector<Node>& n){
    nodes = &n;
}

void Node::print() const{
    std::cout << "Node(id=" << id << ", x=" << pos.x << ", y=" <<  pos.y<< ", z=" <<  pos.z << ")\n";
}


