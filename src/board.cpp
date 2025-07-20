#include <vector>
#include <queue>
#include <set>
#include <algorithm>

#include "raylib.h"
#include "board.h"
#include "node.h"
using std::vector;

#define MAX_NODES 1000

Board::Board() :
    edges(0),
    lastNodeIndex(0),
    currentStep(0),
    isDirected(false),
    isRunning(false),
    lastClickedNode(-1, {0, 0}, 0),
    graph(MAX_NODES),
    visited(MAX_NODES, false),
    nodes(MAX_NODES, Node(-1, {0, 0}, 0)),
    traversalOrder(MAX_NODES)
{}

NodePair Board::findNodesFromPositions(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    NodePair pair; 

    for (Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(firstNodePosition)) {
            pair.firstNode = &node;
        }

        if (node.isNodeValid() && node.isInRadiusDomain(secondNodePosition)) {
            pair.secondNode = &node;
        }
    }

    return pair;
}

Vector2 Board::isInNodeDomain(Vector2 mousePosition) {
    for (const Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(mousePosition)) {
            return node.getNodePosition();
        }
    }

    return {0.0f, 0.0f};
}

void Board::addNode(Vector2 mousePosition) {
    for (const Node& node : nodes) {
        float minDistance = node.getNodeRadius() * 3;

        if (node.isNodeValid()) {
            Vector2 nodePosition = node.getNodePosition(); 

            float dx = mousePosition.x - nodePosition.x;
            float dy = mousePosition.y - nodePosition.y;

            if (dx * dx + dy * dy <= minDistance * minDistance) {
                return;
            }
        }
    }

    Node currentNode = Node(lastNodeIndex, mousePosition, 50.0f);
    nodes[lastNodeIndex++] = currentNode;
}

void Board::addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    NodePair found = findNodesFromPositions(firstNodePosition, secondNodePosition);
    if (!found.firstNode || !found.secondNode) {
        return;
    }

    int firstNodeIndex = found.firstNode->getNodeIndex();
    int secondNodeIndex = found.secondNode->getNodeIndex();

    graph[firstNodeIndex].push_back(secondNodeIndex); 
    found.firstNode->addNeighbor(secondNodeIndex);

    if (!isDirected) {
        graph[secondNodeIndex].push_back(firstNodeIndex);
        found.secondNode->addNeighbor(firstNodeIndex);
    }
}

void Board::removeEdge(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    NodePair found = findNodesFromPositions(firstNodePosition, secondNodePosition);
    if (!found.firstNode || !found.secondNode) {
        return;
    }

    int firstNodeIndex = found.firstNode->getNodeIndex();
    int secondNodeIndex = found.secondNode->getNodeIndex();

    auto& neighbors1 = graph[firstNodeIndex];
    neighbors1.erase(std::remove(neighbors1.begin(), neighbors1.end(), secondNodeIndex), neighbors1.end());
    found.firstNode->removeNeighbor(secondNodeIndex);
   
    if (!isDirected) {
        auto& neighbors2 = graph[secondNodeIndex];
        neighbors2.erase(std::remove(neighbors2.begin(), neighbors2.end(), firstNodeIndex), neighbors2.end());
        found.secondNode->removeNeighbor(firstNodeIndex);
    }
}

void Board::removeNode(Vector2 mousePosition) {
    Node* nodeToRemove = nullptr;
    for (int i = 0; i < lastNodeIndex; i++) {
        if (nodes[i].isNodeValid() && nodes[i].isInRadiusDomain(mousePosition)) {
            nodeToRemove = &nodes[i];
            break;
        }
    }

    if (!nodeToRemove) return;

    graph[nodeToRemove->getNodeIndex()].clear();
    nodeToRemove->removeNeighbors();

    for (int i = 0; i < lastNodeIndex; i++) {
        if (i == nodeToRemove->getNodeIndex()) continue;

        auto& adj = graph[i];
        adj.erase(std::remove(adj.begin(), adj.end(), nodeToRemove->getNodeIndex()), adj.end());
        nodes[i].removeNeighbor(nodeToRemove->getNodeIndex());
    }

    // mark node as invalid
    *nodeToRemove = Node(-1, {0.0f, 0.0f}, 0.0f);
}

void Board::drawNodes() {
    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        Vector2 nodePosition = node.getNodePosition();
        DrawCircle(nodePosition.x, nodePosition.y, node.getNodeRadius(), RED);
    }  
}

void Board::drawEdges() {
    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        std::set<int> neighbors = node.getNodeNeighbors();
        for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
            Node* neighbor = &nodes[*it];
            DrawLineEx(node.getNodePosition(), neighbor->getNodePosition(), 5.0f, GREEN);
        }
    }         
}
