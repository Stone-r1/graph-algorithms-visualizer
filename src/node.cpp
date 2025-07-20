#include "raylib.h"
#include "node.h"

Node::Node(int index, Vector2 position, float radius) {
    nodeIndex = index;
    nodePosition = position;
    nodeRadius = radius;
    isActive = false;
}

int Node::getNodeIndex() const {
    return nodeIndex;
}

Vector2 Node::getNodePosition() const {
    return nodePosition;
}

float Node::getNodeRadius() const {
    return nodeRadius;
}

bool Node::isNodeActive() const {
    return isActive;
}

bool Node::isInRadiusDomain(Vector2 position) const {
    float dx = position.x - nodePosition.x;
    float dy = position.y - nodePosition.y;
    return dx * dx + dy * dy <= nodeRadius * nodeRadius; 
}

void Node::drawNode() const {
    DrawCircle(nodePosition.x, nodePosition.y, nodeRadius, RED);
}
