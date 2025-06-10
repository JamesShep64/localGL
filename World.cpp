#include "World.h"
World::World(){
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    manager.addNode(100,0,100);
    manager.addNode(100,0,0);
    manager.addNode(0,0,0);
}
void World::spawnNode(Node& n){
    Vec3 spawnOrigin = n.pos + Vec3(spawnDisplance, 0, spawnDisplance);
    Node top = Node();
}
