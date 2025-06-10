#pragma once
#include "Octree/octree.h"
#include "Node.h"
#include "glm/glm/ext/vector_float3.hpp"
#include <iostream>
#include <vector>
using NodeBox2D = std::array<Node, 2>;
using NodeRay2D = std::array<Node, 2>;
struct NodePlane2D{float OrigoDistance; Node Normal;};
// Custom adaptor for your Node class
struct NodeAdaptorBasics {
    // Get coordinate component from Node
    static float GetPointC(const Node& node, OrthoTree::dim_t dimension) {
        switch (dimension) {
            case 0: return node.pos.x;
            case 1: return node.pos.z;
            case 2: return node.pos.y;
            default: 
                assert(false && "Invalid dimension for 3D Node");
                return node.pos.x;
        }
    }
    
    // Set coordinate component in Node
    static void SetPointC(Node& node, OrthoTree::dim_t dimension, float value) {
        switch (dimension) {
            case 0: node.pos.x = value; break;
            case 1: node.pos.z = value; break;
            case 2: node.pos.y = value; break;
            default: 
                assert(false && "Invalid dimension for 3D Node");
        }
    }
    
    // Bounding box operations
    static void SetBoxMinC(NodeBox2D& box, OrthoTree::dim_t dimension, float value) { 
        SetPointC(box[0], dimension, value); 
    }
    
    static void SetBoxMaxC(NodeBox2D& box, OrthoTree::dim_t dimension, float value) { 
        SetPointC(box[1], dimension, value); 
    }
    
    static float GetBoxMinC(const NodeBox2D& box, OrthoTree::dim_t dimension) { 
        return GetPointC(box[0], dimension); 
    }
    
    static float GetBoxMaxC(const NodeBox2D& box, OrthoTree::dim_t dimension) { 
        return GetPointC(box[1], dimension); 
    }
    
    // Ray operations
    static const Node& GetRayDirection(const NodeRay2D& ray) { return ray[1]; }
    static const Node& GetRayOrigin(const NodeRay2D& ray) { return ray[0]; }
    
    // Plane operations
    static const Node& GetPlaneNormal(const NodePlane2D& plane) { return plane.Normal; }
    static float GetPlaneOrigoDistance(const NodePlane2D& plane) { return plane.OrigoDistance; }
};

// Define the complete adaptor
using NodeAdaptor = OrthoTree::AdaptorGeneralBase<
    2,              // 3D
    Node,           // Point type
    NodeBox2D,      // Bounding box type
    NodeRay2D,      // Ray type
    NodePlane2D,    // Plane type
    float,          // Coordinate type
    NodeAdaptorBasics
>;

// Define your custom octree types
using NodeQuadPoint = OrthoTree::OrthoTreePoint<
    2,              // 3D
    Node,           // Point type
    NodeBox2D,      // Bounding box type
    NodeRay2D,      // Ray type
    NodePlane2D,    // Plane type
    float,          // Coordinate type
    NodeAdaptor
>;

// Example usage class
class NodeQuadManager {
private:
    std::unique_ptr<NodeQuadPoint> quadtree;
    
public:
    std::vector<Node> nodes;
    std::vector<glm::vec3> vertices;
    NodeQuadManager() = default;
    
    // Add a node to the collection
    void addNode(const Node& node){
        nodes.push_back(node);
    }
    
    // Add a node with coordinates
    void addNode(float x, float y, float z) {
        nodes.emplace_back(x, y, z, nodes.size()+1);
        vertices.emplace_back(x, y, z);
    }
    
    // Build the octree
    void buildQuadtree(int maxDepth = 10, int maxElementsPerNode = 10) {
        if (nodes.empty()) {
            std::cout << "No nodes to build octree with!" << std::endl;
            return;
        }
        
        quadtree = std::make_unique<NodeQuadPoint>(
            nodes, 
            maxDepth, 
            std::nullopt,  // Auto-calculate bounding box
            maxElementsPerNode
        );
        
        std::cout << "Octree built with " << nodes.size() << " nodes" << std::endl;
    }
    
    // Range search - find all nodes within a bounding box
    std::vector<OrthoTree::index_t> rangeSearch(const Node& minCorner, const Node& maxCorner) const {
        if (!quadtree) {
            std::cout << "Octree not built yet!" << std::endl;
            return {};
        }
        
        NodeBox2D searchBox = {minCorner, maxCorner};
        return quadtree->RangeSearch(searchBox, nodes);
    }
    
    // K-nearest neighbors search
    std::vector<OrthoTree::index_t> findNearestNeighbors(const Node& queryPoint, int k) const {
        if (!quadtree) {
            std::cout << "Octree not built yet!" << std::endl;
            return {};
        }
        
        return quadtree->GetNearestNeighbors(queryPoint, k, nodes);
    }

    // Get node by index
    const Node& getNode(OrthoTree::index_t index) const {
        return nodes[index];
    }
    
    // Get all nodes
    const std::vector<Node>& getAllNodes() const {
        return nodes;
    }

    const std::vector<glm::vec3> &getVertices() const {
        return vertices;
    }
    // Print search results
    void printSearchResults(const std::vector<OrthoTree::index_t>& indices, const std::string& searchType) const {
        std::cout << searchType << " found " << indices.size() << " nodes:" << std::endl;
        for (int idx : indices) {
            std::cout << "  Index " << idx << ": ";
            nodes[idx].print();
        }
    }
    
    // Clear all data
    void clear() {
        nodes.clear();
        quadtree.reset();
    }
};
