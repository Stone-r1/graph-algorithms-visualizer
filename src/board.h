#include <vector>
#include <queue>
#include "raylib.h"
using std::vector;

class Board {
private:
    int edges;
    int nodes;
    int currentStep;
    bool isDirected;
    bool isRunning;

    vector<vector<int>> graph;
    vector<bool> visited;
    vector<Vector2> nodePositions;
    vector<int> traversalOrder;
    std::queue<int> BFSQueue;

public:
    Board();

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
};
