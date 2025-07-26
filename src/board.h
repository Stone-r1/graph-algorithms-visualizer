#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <memory>

#include "raylib.h"
#include "node.h"
#include "algorithms/bfs.h"
#include "algorithms/dfs.h"
using std::vector;


class Board {
private:
    int edges;
    int lastNodeIndex;
    int currentStep;
    bool isDirected;
    bool isWeighted;
    bool isRunning;
    Node lastClickedNode;

    bool isEnteringWeight = false;
    char weightInput[10] = "";
    int weightDigitCount = 0;
    bool weightReady = false;

    vector<vector<int>> graph;
    vector<Node> nodes;
    std::set<pair<int, int>> highlightedEdges;

    std::unique_ptr<TraversalAlgorithm> currentAlgo;

    Node* findNodeFromPosition(Vector2 firstNodePosition);

public:
    Board();

    Vector2 isInNodeDomain(Vector2 mousePosition);

    void addNode(Vector2 mousePosition, float currentRadius);
    void addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition, int weight = 1);
    void removeEdge(Vector2 firstNodePosition, Vector2 secondNodePosition);
    void removeNode(Vector2 mousePosition);
    void clearGraph();

    // ==== algorithms ====
    void runBFS(Vector2 startNodePosition);
    void runDFS(Vector2 startNodePosition);

    void stepForward();
    void stepBackward();
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

    bool isGraphEmpty() const;
    bool isGraphWeighted() const;
 
    void flipGraphWeight();
    void askForWeight();
    int getCurrentWeight();
    void setWeightReady();
    bool isWeightReady() const;
};
