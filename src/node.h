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

public:
    Node(int index, Vector2 position, float radius);
    Node();
    
    int getNodeIndex() const;
    Vector2 getNodePosition() const;
    float getNodeRadius() const;

    bool isNodeActive() const;
    bool isNodeValid() const;
    bool isNeighborValid(int nodeID) const;
    bool isInRadiusDomain(Vector2 mousePosition) const;

    void addNeighbor(int nodeID);
    void changeNodeStatus();
    void drawNode() const;
};

#endif
