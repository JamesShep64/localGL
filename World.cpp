#include "World.h"
#include "Quad.h"
#include "Vec3.h"
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
    Vec3 spawnDir = normalize(((n.pos - n.A->pos) + (n.pos-n.B->pos)));
    Vec3 spawnOrigin = n.pos + spawnDir * spawnDisplance;
    Vec3 offset =  spawnDir * spawnRange + perpindicular(spawnDir) * spawnRange;

    Node spawnPoint = Node(spawnOrigin + offset * dist(gen),0);

    Node top = Node((spawnPoint.pos + offset * 8),0); 
    Node bottom = Node((spawnPoint.pos - offset * 8),0);

    std::vector<uint> found; 
    found = manager.rangeSearch(bottom, top);
    std::cout<<found.size()<<" <- Num nodes Found\n";
    std::cout<<"SPAWN POINT: ";
    spawnPoint.print();

    bool spawn_collision = false;
    std::cout<<"\nFOUND NODES";
    for(OrthoTree::index_t i : found){
    std::cout<<i;
        auto node = manager.nodes[i];
        if(spawn_collision){
            //std::cout<<"=======================SPAWN COLLISION==================\n";
            spawn_collision = true;
            break;
        }
    }
    std::cout<<"\n";
    //std::cout<<"TEST: "<<manager.nodes[found[0]].id<<" | "<<manager.nodes[manager.findNearestNeighbors(n,1)[0]].id<<"\n"
    if(!spawn_collision){
        
        manager.addNode(spawnPoint.pos.x, spawnPoint.pos.y, spawnPoint.pos.z);
        Node& newNode = manager.nodes.back();
        newNode.print();
        auto other_vertex = n.getConnectionVertex(newNode);
        newNode.setVert(n);
        newNode.setVert(*other_vertex);
        indices.push_back(newNode.id);
        indices.push_back(newNode.A->id);
        indices.push_back(newNode.B->id);
        std::cout<<"NEW TRIANGLE: "<<indices[indices.size()-3]<<" | "<<indices[indices.size()-2]<<" | "<<indices[indices.size()-1]<<"\n";
        std::cout<<"NEW NODE ADDED\n";
    }
    
}
