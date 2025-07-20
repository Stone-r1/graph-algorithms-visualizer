#include <vector>
#include <queue>
#include "raylib.h"
#include "node.h"
using std::vector;

class Board {
private:
    int edges;
    int lastNodeIndex;
    int currentStep;
    bool isDirected;
    bool isRunning;
    Node lastClickedNode;

    vector<vector<int>> graph;
    vector<bool> visited;
    vector<Node> nodes;
    vector<int> traversalOrder;

public:
    Board();

    int getEdges() const;

    void addNode(Vector2 mousePosition);
    void addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition);
    void removeEdge(Node node1, Node node2);
    void removeNode(Node node);
    bool hasEdge(Node node1, Node node2);
    void clearGraph();

    void stopRunning();
    void resetRunning();

    void draw();
    void drawTraversal();
};
