#include <vector>
#include <queue>
#include "raylib.h"
#include "board.h"
#include "node.h"
#include <iostream>
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
    graph(MAX_NODES, vector<int>(MAX_NODES)),
    visited(MAX_NODES, false),
    nodes(MAX_NODES, Node(-1, {0, 0}, 0)),
    traversalOrder(MAX_NODES)
{}

void Board::addNode(Vector2 mousePosition) {
    Node currentNode = Node(lastNodeIndex, mousePosition, RADIUS);
    nodes[lastNodeIndex++] = currentNode;
}

void Board::addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    // first guess what indeces do these nodes have
    int firstNodeIndex = -1, secondNodeIndex = -1;
    for (const Node& nodes : nodes) {
        if (nodes.isInRadiusDomain(firstNodePosition)) {
            firstNodeIndex = nodes.getNodeIndex();
        }

        if (nodes.isInRadiusDomain(secondNodePosition)) {
            secondNodeIndex = nodes.getNodeIndex();
        }
    }

    if (firstNodeIndex != -1 && secondNodeIndex != -1) {
        graph[firstNodeIndex].push_back(secondNodeIndex); 
        if (!isDirected) {
            graph[secondNodeIndex].push_back(firstNodeIndex);
        }
    } // in practice it's impossible to have "else" scenario.
}

/*
    Board();

    int getEdges() const;

    void addNode(Vector2 mousePosition);
    void addEdge(Node node1, Node node2);
    void removeEdge(Node node1, Node node2);
    void removeNode(Node node);
    bool hasEdge(Node node1, Node node2);
    void clearGraph();

    void stopRunning();
    void resetRunning();

    void draw();
    void drawTraversal();
};
*/

