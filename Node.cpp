#include "Node.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

std::vector<Node>* Node::nodes = nullptr;
Node::Node(Vec3 pos, int id){
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
    connections.insert(a);
    connections.insert(b);
    addTriangle(a->id,b->id, id);
}

bool Node::connectToArea(std::vector<uint> found){
    bool col_found = false;
    if(Node::nodes == nullptr){std::cerr<<"NODES NULL\n";return false;}
    std::vector<Ray> rays;
    if(found.size() < 2)
        return false;
    for(int i = 0; i < found.size()-2; i++){
        auto outer_it = getId_map().find(found[i]);
        if (outer_it != getId_map().end()){
            for(int j = i + 1; j < found.size()-1; j++){
                auto inner_it = outer_it->second.find(j);
                if(inner_it != outer_it->second.end()){
                    for(int k = j + 1; k < found.size(); k++){
                        auto it = std::find(inner_it->second.begin(), inner_it->second.end(), k);
                        if (it != inner_it->second.end()){
                            Ray ray1, ray2, ray3;
                            ray1.n = &(*Node::nodes)[i];
                            ray1.zero = (*Node::nodes)[i].pos;
                            ray1.vec = (*Node::nodes)[j].pos-(*Node::nodes)[i].pos;
                            ray2.n = &(*Node::nodes)[j];
                            ray2.zero = (*Node::nodes)[j].pos;
                            ray2.vec = (*Node::nodes)[k].pos-(*Node::nodes)[j].pos;
                            ray3.n = &(*Node::nodes)[k];
                            ray3.zero = (*Node::nodes)[k].pos;
                            ray3.vec = (*Node::nodes)[i].pos-(*Node::nodes)[k].pos;
                            rays.push_back(ray1);
                            rays.push_back(ray2);
                            rays.push_back(ray3);
                        } 
                    }
                }
            }
        }
    }
    std::unordered_set<Node*> connecting_nodes;
    bool no_collision = true;
    for(int i=0; i < rays.size(); i++){
        no_collision = true;
        for(int j=0; j < rays.size(); j++){
            if(j == i ||  ((j > i - 2) && 
                ((i%3==0 && j == i + 2) || 
                 (i%3==1 && j == i - 1) ||
                 (i%3==2 && j == i - 1))))
                continue;
            if(checkRayIntersection2D(rays[i].zero, rays[j])){
                no_collision = false;
                break;
            }
        }
        if(no_collision){
            connecting_nodes.insert(rays[i].n);
        }
    }

    for(Node* n : connecting_nodes){
        std::vector<Node*> results;
        std::set_intersection(n->connections.begin(),n->connections.end(),connecting_nodes.begin(),connecting_nodes.end(),std::back_inserter(results));
        for(Node* r : results){
            col_found = true;
            addConnections(n, r);
        }
    }
    return col_found;
}

bool Node::checkRayIntersection2D(const Vec3& point,const Ray& ray){
    auto t_u = Node::vectorIntersection2D(pos, pos-point, ray.zero, ray.vec);
    if(t_u[0] < 1 && t_u[0] > 0 && t_u[1] < 1 && t_u[1] > 0)
        return true;
    return false;
}
std::array<float, 2> Node::vectorIntersection2D(const Vec3& zero_t, const Vec3& vec_t,const Vec3& zero_u,const Vec3& vec_u){
    float h = vec_t.x * vec_u.z - vec_t.z * vec_u.x;
    float t = (zero_u.x - zero_t.x) * vec_u.z - (zero_u.z - zero_t.z) * vec_u.x;
    t/=h;
    
    float u = (zero_u.x - zero_t.x) * vec_t.z - (zero_u.z - zero_t.z) * vec_t.x;
    u/=h;
    return std::array<float,2>{t,u};
}

std::array<int,3> Node::sort_ids(const int& a,const int& b,const int& c){
    int smallest = std::min({a, b,c});
    int largest  = std::max({a, b,c});
    int middle   = a+ b+ c - smallest - largest;
    return {smallest, middle, largest};
}

bool Node::check_triangle(int& a, int& b, int& c){
    auto sorted = sort_ids(a,b,c);
    auto outer_it = getId_map().find(sorted[0]);
    if (outer_it != getId_map().end()) {
        auto inner_it = outer_it->second.find(sorted[1]);
        if (inner_it != outer_it->second.end()) {
            const auto& vec = inner_it->second;
            return std::find(vec.begin(), vec.end(), sorted[2]) != vec.end();
        }
    }
    return false;
}

void Node::addTriangle(const int& a,const int& b,const int& c){
    if(a == b || a == c || b ==c){
        std::cerr<<"ADDING TRI WITH REPEAT NODE\n";
        return;
    }
    auto sorted = sort_ids(a, b, c);
    std::cout<<sorted[0]<<" "<<sorted[1]<<" "<<sorted[2]<<" TRI ADDED\n";
    getId_map()[sorted[0]][sorted[1]].insert(sorted[2]);
}

void Node::print() const{
    std::cout << "Node(id=" << id << ", x=" << pos.x << ", y=" <<  pos.y<< ", z=" <<  pos.z << ")\n";
}


