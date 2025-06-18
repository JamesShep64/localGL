#include "World.h"
#include "Quad.h"
#include "Vec3.h"
#include <unordered_map>
World::World(): gen(rd()), dist(-1.0f, 1.0f){
    Node::nodes = &manager.nodes;
    manager.addNode(100,0,100);
    manager.addNode(100,0,0);
    manager.addNode(0,0,0);

    manager.addNode(300,0,300);
    manager.addNode(300,0,0);
    manager.addNode(200,0,200);

    manager.buildQuadtree();
    manager.nodes[0].addConnections(&manager.nodes[2],&manager.nodes[1]);
    manager.nodes[1].addConnections(&manager.nodes[2],&manager.nodes[0]);
    manager.nodes[2].addConnections(&manager.nodes[0],&manager.nodes[1]);

    manager.nodes[3].addConnections(&manager.nodes[5],&manager.nodes[4]);
    manager.nodes[4].addConnections(&manager.nodes[5],&manager.nodes[3]);
    manager.nodes[5].addConnections(&manager.nodes[3],&manager.nodes[4]);
    this->setVertices();
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
    if(n.connections.size() == 0){
        std::cerr<<"NODE HAS NO CONNECTIONS\n";
        return;
    }
    auto it = n.connections.begin();
    Node* a = *it;
    ++it;
    Node* b = *it;
    Vec3 spawnDir = normalize(((n.pos - a->pos) + (n.pos - b->pos)));
    Vec3 spawnOrigin = n.pos + spawnDir * spawnDisplance;
    Vec3 offset =  spawnDir * spawnRange + perpindicular(spawnDir) * spawnRange;

    Node spawnPoint = Node(spawnOrigin + offset * dist(gen),manager.nodes.size());

    topSpawnCorner = spawnPoint.pos + offset * 50;
    bottomSpawnCorner= spawnPoint.pos - offset * 50;
    Node top = Node((topSpawnCorner),0); 
    Node bottom = Node((bottomSpawnCorner),0);

    auto found = manager.rangeSearch(bottom, top);
    //std::cout<<found.size()<<" <- Num nodes Found\n";
    //std::cout<<"SPAWN POINT: ";
    spawnPoint.print();
    //std::cout<<&Node::getId_map()<<"   "<<Node::getId_map()[0][1].size()<<" WC\n";
    if(spawnPoint.connectToArea(found)){
        std::cout<<"ADDED\n";
        manager.addNode(spawnPoint);
    }
    setVertices();
}

void World::setVertices(){
    indices.clear();
    for(const auto& outer : Node::getId_map()){
        for(const auto& inner : outer.second){
            for(int index : inner.second){
                indices.push_back(outer.first);
                indices.push_back(inner.first);
                indices.push_back(index);
                std::cout<<"TRI: "<<outer.first<<"-"<<inner.first<<"-"<<index<<"\n";
            }
        }
    }
    std::cout<<"LEN INDEICES: "<<indices.size()<<"\n";
    std::cout<<"LEN VERTEXES: "<<manager.vertices.size()<<"\n";
}
