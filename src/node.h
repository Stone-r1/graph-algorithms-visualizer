#ifndef NODE_H
#define NODE_H

#include "raylib.h"
#include <set>

class Node {
private:
    int nodeIndex;
    Vector2 nodePosition;
    float nodeRadius;
    bool isActive;
    std::set<int> neighbors;

    bool isHighlighted = false;

public:
    Node(int index, Vector2 position, float radius);
    Node();
    
    int getNodeIndex() const;
    Vector2 getNodePosition() const;
    float getNodeRadius() const;
    std::set<int> getNodeNeighbors() const;

    bool isNodeActive() const;
    bool isNodeValid() const;
    bool isNeighborValid(int nodeID) const;
    bool isInRadiusDomain(Vector2 mousePosition) const;

    void addNeighbor(int nodeID);
    void removeNeighbor(int nodeID);
    void removeNeighbors();
    void changeNodeStatus();
    void drawNode() const;

    void setHighlight(); 
    void resetHighlight(); 
    bool highlighted() const;
};

#endif
