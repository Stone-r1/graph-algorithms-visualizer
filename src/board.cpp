#include <vector>
#include <queue>
#include "raylib.h"
#include "board.h"
#include <iostream>
using std::vector;

#define MAX_NODES 1000
#define RADIUS 50

Board::Board() {
    edges = 0;
    nodes = 0;
    lastNode = 0;
    currentStep = 0;
    isDirected = false;
    isRunning = false;

    graph.assign(MAX_NODES, vector<int>(MAX_NODES));
    visited.resize(MAX_NODES, false);
    traversalOrder.resize(MAX_NODES);
    nodePositions.resize(MAX_NODES);
    nodePositions.clear();
}

void Board::draw() {
    for (Vector2 mouse : nodePositions) {
        DrawCircle(mouse.x, mouse.y, RADIUS, RED);
    }
}

void Board::addNode(Vector2 mouse) {
    nodePositions.emplace_back(mouse);
    lastNode++;
}

void Board::addEdge(int node1, int node2) {
    graph[node1].push_back(node2);
    if (!isDirected) {
        graph[node2].push_back(node1);
    }
}

bool Board::isInNodeDomain(Vector2 mouse) {
    for (Vector2 position : nodePositions) {
        float dx = mouse.x - position.x;
        float dy = mouse.y - position.y;
        float distanceSquared = dx * dx - dy * dy;
        
        if (RADIUS * RADIUS >= distanceSquared) {
            std::cout << "YES\n";
            lastClickedNode = position;
            return true;
        }
    }

    return false;
}

/*
int Board::getNode() const {
    return lastClickedNode;
}
*/


/*
private:
    int edges = 0;
    int nodes = 0;
    int lastNode = 0;
    int currentStep = 0;
    bool isDirected = false;
    bool isRunning = false;

    vector<vector<int>> graph;
    vector<bool> visited;
    vector<Vector2> nodePositions;
    vector<int> traversalOrder;
    std::queue<int> BFSQueue;

public:
    int getEdges() const;
    void increaseEdges();
    void clearEdges();
    
    int getNodes() const;
    void increaseNodes();
    void clearNods();

    void addNode(Vector2 pos);
    void addEdge(int node1, int node2);
    void removeEdge(int node1, int node2);
    bool hasEdge(int node1, int node2);
    void clearGraph();

    void startDFS(int firstNode);
    void startBFS(int firstNode);
    void stopRunning();
    void resetRunning();

    void draw();
    void drawTraversal();
*/

