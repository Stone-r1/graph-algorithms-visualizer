#include "node.h"
#include "data/constants.h"


Node::Node(int index, Vector2 position, float radius = NodeConstants::MEDIUM_RADIUS) :
    nodeIndex(index),
    nodePosition(position),
    nodeRadius(radius),
    isActive(false)
{}

Node::Node() : 
    nodeIndex(-1),
    nodePosition({0, 0}),
    nodeRadius(0.0f),
    isActive(false)
{}

int Node::getNodeIndex() const {
    return nodeIndex;
}

Vector2 Node::getNodePosition() const {
    return nodePosition;
}

float Node::getNodeRadius() const {
    return nodeRadius;
}

const std::unordered_set<int>& Node::getNodeNeighbors() const {
    return neighbors;
}

bool Node::isNodeActive() const {
    return isActive;
}

bool Node::isNodeValid() const {
    return !(nodeIndex == -1); 
}

bool Node::isNeighborValid(int nodeID) const {
    return (neighbors.find(nodeID) != neighbors.end());
}

bool Node::isInRadiusDomain(Vector2 position) const {
    float dx = position.x - nodePosition.x;
    float dy = position.y - nodePosition.y;
    return (dx * dx + dy * dy <= nodeRadius * nodeRadius); 
}

void Node::addNeighbor(int nodeID) {
    neighbors.insert(nodeID);
}

void Node::removeNeighbor(int nodeID) {
    neighbors.erase(nodeID);
}

void Node::removeNeighbors() {
    neighbors.clear();
}

void Node::changeNodeStatus() {
    isActive = !isActive;
}

void Node::drawNode() const {
    DrawCircle(nodePosition.x, nodePosition.y, nodeRadius, ColorConstants::NODE_VISITED);
}

void Node::setHighlight() {
    isHighlighted = true;
}

void Node::resetHighlight() {
    isHighlighted = false;
}

bool Node::highlighted() const {
    return isHighlighted;
}

Node Node::makeInvalidNode() {
    return Node(-1, {0, 0}, 0);
}
