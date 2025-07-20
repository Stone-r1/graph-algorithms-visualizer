#ifndef NODE_H
#define NODE_H

#include "raylib.h"

class Node {
private:
    int nodeIndex;
    Vector2 nodePosition;
    float nodeRadius;
    bool isActive;

public:
    Node(int index, Vector2 position);
    
    int getNodeIndex() const;
    Vector2 getNodePosition() const;
    float getNodeRadius() const;
    bool isNodeActive() const;

    int isInRadiusDomain(const Node& node, Vector2 mousePosition);
    void drawNode(const Node& node, Vector2 mousePosition);
};

#endif
