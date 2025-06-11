#include "World.h"
#include "Quad.h"
World::World(): gen(rd()), dist(-1.0f, 1.0f){
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    manager.addNode(100,0,100);
    manager.addNode(100,0,0);
    manager.addNode(0,0,0);
    manager.nodes[0].setVert(manager.nodes[1]);
    manager.nodes[0].setVert(manager.nodes[2]);
    manager.nodes[1].setVert(manager.nodes[0]);
    manager.nodes[1].setVert(manager.nodes[2]);
    manager.nodes[2].setVert(manager.nodes[0]);
    manager.nodes[2].setVert(manager.nodes[1]);
    manager.buildQuadtree();
    spawnIndex = manager.nodes.size()-1;
    printf("WORLD INITED\n");
}
void World::doSpawn(){
    spawnNode(manager.nodes[spawnIndex]);
    spawnIndex--;
    if(spawnIndex < 0)
        spawnIndex = manager.nodes.size()-1;
}
void World::spawnNode(Node& n){
    Vec3 offset = Vec3(spawnRange, 0, spawnRange);
    Vec3 spawnOrigin = n.pos + Vec3(spawnDisplance, 0, spawnDisplance);
    Node spawnPoint = Node(spawnOrigin.x+spawnRange * dist(gen), 0, spawnOrigin.z + spawnRange * dist(gen), 0);
    Node top = Node((spawnOrigin + offset),0);
    Node bottom = Node((spawnOrigin - offset),0);
    auto found = manager.rangeSearch(bottom, top);
    std::cout<<"SPAWN POINT: ";
    spawnPoint.pos.print();
    bool spawn_collision = false;
    for(OrthoTree::index_t i : found){
        auto node = manager.nodes[i];
        std::cout<<"Checking node: "<<i<<" id = "<<node.id<<"\n";
        spawn_collision = Node::checkWithin(&node, spawnPoint);
        if(spawn_collision){
            std::cout<<"SPAWN COLLISION\n";
            break;
        }
    }
    if(!spawn_collision){
        manager.addNode(spawnPoint.pos.x, spawnPoint.pos.y, spawnPoint.pos.z);
        Node& newNode = manager.nodes.back();
        newNode.setVert(n);
        auto other_vertex = n.getClosestVertex(newNode);
        newNode.setVert(n);
        indices.push_back(newNode.id);
        indices.push_back(newNode.A->id);
        indices.push_back(newNode.B->id);
        std::cout<<"NEW NODE ADDED\n";
    }
    
}
