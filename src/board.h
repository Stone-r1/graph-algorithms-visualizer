#include <vector>
#include <set>
#include <unordered_map>
#include <memory>
#include <optional>

#include "raylib.h"
#include "node.h"
#include "algorithms/BFS/bfs.h"
#include "algorithms/DFS/dfs.h"
#include "algorithms/Dijkstra/dijkstra.h"
#include "algorithms/Bellman-Ford/bellmanFord.h"
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
    bool isInBoardBorderDomain(const Vector2& mousePosition, const float& currentRadius,
                               const int& screenWidth, const int& screenHeight);

    void addNode(const Vector2& mousePosition, const float& currentRadius);
    void addEdge(const Vector2& firstNodePosition, const Vector2& secondNodePosition, int weight = 1);
    void removeEdge(const Vector2& firstNodePosition, const Vector2& secondNodePosition);
    void removeNode(const Vector2& mousePosition);
    void clearGraph();

    // ==== algorithms ====
    bool runBFS(const Vector2& startNodePosition);
    bool runDFS(const Vector2& startNodePosition);
    bool runDijkstra(const Vector2& startNodePosition);
    bool runBellmanFord(const Vector2& startNodePosition);

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
    void drawWeights(const Font& font);

    bool isGraphEmpty() const;
    bool isGraphWeighted() const;
 
    void flipGraphWeight();
    void askForWeight();
    int getCurrentWeight();
    void setWeightReady();
    bool isWeightReady() const;
};
