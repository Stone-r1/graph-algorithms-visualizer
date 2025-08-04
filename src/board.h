#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <memory>
#include <optional>

#include "raylib.h"
#include "node.h"
#include "algorithms/bfs.h"
#include "algorithms/dfs.h"
#include "algorithms/dijkstra.h"
#include "algorithms/bellmanFord.h"
using std::vector;


class Board {
private:
    int edges;
    int lastNodeIndex;
    int startNodeIndex = -1;
    int currentStep;
    bool isDirected;
    bool isWeighted;
    bool isRunning;
    Node lastClickedNode;

    bool isEnteringWeight = false;
    char weightInput[10] = "";
    int weightDigitCount = 0;
    bool weightReady = false;

    vector<vector<pair<int, int>>> graph;
    vector<Node> nodes;
    std::set<pair<int, int>> highlightedEdges;
    std::unordered_map<int, int> highlightedWeights;

    std::unique_ptr<TraversalAlgorithm> currentAlgo;

    Node* findNodeFromPosition(const Vector2& firstNodePosition);

public:
    Board();

    void clear();

    std::optional<Vector2> isInNodeDomain(const Vector2& mousePosition);

    void addNode(const Vector2& mousePosition, const float& currentRadius);
    void addEdge(const Vector2& firstNodePosition, const Vector2& secondNodePosition, int weight = 1);
    void removeEdge(const Vector2& firstNodePosition, const Vector2& secondNodePosition);
    void removeNode(const Vector2& mousePosition);
    void clearGraph();

    // ==== algorithms ====
    void runBFS(const Vector2& startNodePosition);
    void runDFS(const Vector2& startNodePosition);
    void runDijkstra(const Vector2& startNodePosition);
    void runBellmanFord(const Vector2& startNodePosition);

    void stepForward();
    void stepBackward();
    // ====================
    
    // ==== highlights ====
    void highlightNode(const int& index);
    void highlightEdge(const int& from, const int& to);
    void highlightWeight(const int& to, const int& weight);
    void highlightStartingNode(const Vector2& mousePosition);
    void resetHighlights();
    // ====================

    void stopRunning();
    void resetRunning();
    bool isAlgorithmRunning() const;

    void drawNodes();
    void drawEdges();
    void drawWeights();

    bool isGraphEmpty() const;
    bool isGraphWeighted() const;
 
    void flipGraphWeight();
    void askForWeight();
    int getCurrentWeight();
    void setWeightReady();
    bool isWeightReady() const;
};
