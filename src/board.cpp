#include <vector>
#include <queue>
#include "raylib.h"
#include "board.h"
#include "node.h"
#include <algorithm>
using std::vector;

#define MAX_NODES 1000
#define RADIUS 50


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
        if (node.isInRadiusDomain(firstNodePosition)) {
            pair.firstNode = &node;
        }

        if (node.isInRadiusDomain(secondNodePosition)) {
            pair.secondNode = &node;
        }
    }

    return pair;
}

void Board::addNode(Vector2 mousePosition) {
    Node currentNode = Node(lastNodeIndex, mousePosition, RADIUS);
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
    Node* nodeToRemove;
    for (int i = 0; i < lastNodeIndex; i++) {
        if (nodes[i].isInRadiusDomain(mousePosition)) {
            nodeToRemove = &nodes[i];
            break;
        }
    }

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

/*
    Board();

    int getEdges() const;

    void clearGraph();

    void stopRunning();
    void resetRunning();

    void draw();
    void drawTraversal();
};
*/

