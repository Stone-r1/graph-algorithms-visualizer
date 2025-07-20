#include <vector>
#include <queue>
#include <unordered_map>
#include "raylib.h"
#include "node.h"
using std::vector;


struct NodePair {
    Node* firstNode = nullptr;
    Node* secondNode = nullptr;
};


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

    NodePair findNodesFromPositions(Vector2 firstNodePosition, Vector2 secondNodePosition);

public:
    Board();

    int getEdges() const;

    void addNode(Vector2 mousePosition);
    void addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition);
    void removeEdge(Vector2 firstNodePosition, Vector2 secondNodePosition);
    void removeNode(Vector2 mousePosition);
    void clearGraph();

    void stopRunning();
    void resetRunning();

    void drawNodes();
    void drawEdges();
    void drawTraversal();
};
