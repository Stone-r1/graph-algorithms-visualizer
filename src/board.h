#include <vector>
#include <queue>
#include "raylib.h"
using std::vector;

class Board {
private:
    int edges;
    int nodes;
    int lastNode;
    int currentStep;
    bool isDirected;
    bool isRunning;
    Vector2 lastClickedNode;

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
    
    int getNode() const;
    void increaseNodes();
    void clearNods();

    void addNode(Vector2 pos);
    void addEdge(int node1, int node2);
    void removeEdge(int node1, int node2);
    bool hasEdge(int node1, int node2);
    void clearGraph();

    bool isInNodeDomain(Vector2 mouse);

    void startDFS(int firstNode);
    void startBFS(int firstNode);
    void stopRunning();
    void resetRunning();

    void draw();
    void drawTraversal();
};
