#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <memory>

#include "raylib.h"
#include "node.h"
#include "algorithms/bfs.h"
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
    vector<Node> nodes;
    vector<int> traversalOrder;
    std::set<std::pair<int, int>> highlightedEdges;

    std::unique_ptr<BFS> bfs;

    Node* findNodeFromPosition(Vector2 firstNodePosition);

public:
    Board();

    Vector2 isInNodeDomain(Vector2 mousePosition);

    void addNode(Vector2 mousePosition, float currentRadius);
    void addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition);
    void removeEdge(Vector2 firstNodePosition, Vector2 secondNodePosition);
    void removeNode(Vector2 mousePosition);
    void clearGraph();

    // ==== algorithms ====
    void runBFS(Vector2 startNodePosition);
    void stepBFS();
    // ====================
    
    // ==== highlights ====
    void highlightNode(int index);
    void highlightEdge(int from, int to);
    void resetHighlights();
    // ====================

    void stopRunning();
    void resetRunning();

    void drawNodes();
    void drawEdges();
    void drawTraversal();
};
