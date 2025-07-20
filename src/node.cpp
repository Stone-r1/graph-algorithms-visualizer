#include "raylib.h"
#include "node.h"

Node::Node(int index, Vector2 position) {
    nodeIndex = index;
    nodePosition = position;
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

bool isInRadiusDomain(const Node& node, Vector2 position) {
    Vector2 nodePosition = node.getNodePosition();
    float nodeRadius = node.getNodeRadius();

    float dx = position.x - nodePosition.x;
    float dy = position.y - nodePosition.y;
    return dx * dx - dy * dy <= nodeRadius * nodeRadius; 
}

void drawNode(const Node& node, Vector2 position) {
    DrawCircle(position.x, position.y, node.getNodeRadius(), RED);
}
