#pragma once
#include "Octree/octree.h"
#include "Node.h"
#include <iostream>
using NodeBox2D = std::array<Node, 2>;
using NodeRay2D = std::array<Node, 2>;
using NodeBox3D = std::array<Node, 2>;  // Bounding box defined by min and max Node points
using NodeRay3D = std::array<Node, 2>;  // Ray defined by origin and direction Node points
struct NodePlane3D { 
    float OrigoDistance; 
    Node Normal; 
};

// Custom adaptor for your Node class
struct NodeAdaptorBasics {
    // Get coordinate component from Node
    static float GetPointC(const Node& node, OrthoTree::dim_t dimension) {
        switch (dimension) {
            case 0: return node.x;
            case 1: return node.y;
            case 2: return node.z;
            default: 
                assert(false && "Invalid dimension for 3D Node");
                return node.x;
        }
    }
    
    // Set coordinate component in Node
    static void SetPointC(Node& node, OrthoTree::dim_t dimension, float value) {
        switch (dimension) {
            case 0: node.x = value; break;
            case 1: node.y = value; break;
            case 2: node.z = value; break;
            default: 
                assert(false && "Invalid dimension for 3D Node");
        }
    }
    
    // Bounding box operations
    static void SetBoxMinC(NodeBox3D& box, OrthoTree::dim_t dimension, float value) { 
        SetPointC(box[0], dimension, value); 
    }
    
    static void SetBoxMaxC(NodeBox3D& box, OrthoTree::dim_t dimension, float value) { 
        SetPointC(box[1], dimension, value); 
    }
    
    static float GetBoxMinC(const NodeBox3D& box, OrthoTree::dim_t dimension) { 
        return GetPointC(box[0], dimension); 
    }
    
    static float GetBoxMaxC(const NodeBox3D& box, OrthoTree::dim_t dimension) { 
        return GetPointC(box[1], dimension); 
    }
    
    // Ray operations
    static const Node& GetRayDirection(const NodeRay3D& ray) { return ray[1]; }
    static const Node& GetRayOrigin(const NodeRay3D& ray) { return ray[0]; }
    
    // Plane operations
    static const Node& GetPlaneNormal(const NodePlane3D& plane) { return plane.Normal; }
    static float GetPlaneOrigoDistance(const NodePlane3D& plane) { return plane.OrigoDistance; }
};

// Define the complete adaptor
using NodeAdaptor = OrthoTree::AdaptorGeneralBase<
    3,              // 3D
    Node,           // Point type
    NodeBox3D,      // Bounding box type
    NodeRay3D,      // Ray type
    NodePlane3D,    // Plane type
    float,          // Coordinate type
    NodeAdaptorBasics
>;

// Define your custom octree types
using NodeOctreePoint = OrthoTree::OrthoTreePoint<
    3,              // 3D
    Node,           // Point type
    NodeBox3D,      // Bounding box type
    NodeRay3D,      // Ray type
    NodePlane3D,    // Plane type
    float,          // Coordinate type
    NodeAdaptor
>;

// Example usage class
class NodeOctreeManager {
private:
    std::vector<Node> nodes;
    std::unique_ptr<NodeOctreePoint> octree;
    
public:
    NodeOctreeManager() = default;
    
    // Add a node to the collection
    void addNode(const Node& node){
        nodes.push_back(node);
    }
    
    // Add a node with coordinates
    void addNode(float x, float y, float z) {
        nodes.emplace_back(Node(x, y, z ));
    }
    
    // Build the octree
    void buildOctree(int maxDepth = 10, int maxElementsPerNode = 10) {
        if (nodes.empty()) {
            std::cout << "No nodes to build octree with!" << std::endl;
            return;
        }
        
        octree = std::make_unique<NodeOctreePoint>(
            nodes, 
            maxDepth, 
            std::nullopt,  // Auto-calculate bounding box
            maxElementsPerNode
        );
        
        std::cout << "Octree built with " << nodes.size() << " nodes" << std::endl;
    }
    
    // Range search - find all nodes within a bounding box
    std::vector<OrthoTree::index_t> rangeSearch(const Node& minCorner, const Node& maxCorner) const {
        if (!octree) {
            std::cout << "Octree not built yet!" << std::endl;
            return {};
        }
        
        NodeBox3D searchBox = {minCorner, maxCorner};
        return octree->RangeSearch(searchBox, nodes);
    }
    
    // K-nearest neighbors search
    std::vector<OrthoTree::index_t> findNearestNeighbors(const Node& queryPoint, int k) const {
        if (!octree) {
            std::cout << "Octree not built yet!" << std::endl;
            return {};
        }
        
        return octree->GetNearestNeighbors(queryPoint, k, nodes);
    }

    // Get node by index
    const Node& getNode(OrthoTree::index_t index) const {
        return nodes[index];
    }
    
    // Get all nodes
    const std::vector<Node>& getAllNodes() const {
        return nodes;
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
        octree.reset();
    }
};
