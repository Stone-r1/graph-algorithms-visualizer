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
    nodes(MAX_NODES),
    graph(MAX_NODES, vector<int>(MAX_NODES)),
    visited(MAX_NODES, false),
    nodePositions(MAX_NODES, Node(-1, {0, 0}, 0)),
    traversalOrder(MAX_NODES)
{}

void Board::addNode(Vector2 mousePosition) {
    Node currentNode = Node(lastNodeIndex, mousePosition, RADIUS);
    nodePositions[lastNodeIndex] = currentNode;


}

/*
private:
    int edges;
    int lastNodeIndex;
    int currentStep;
    bool isDirected;
    bool isRunning;
    Node lastClickedNode;

    vector<vector<Node>> graph;
    vector<bool> visited;
    vector<Node> nodePositions;
    vector<int> traversalOrder;

public:
    Board();

    int getEdges() const;
    void increaseEdges();    
    void increaseLastNodeIndex();

    void addNode(Node node);
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

