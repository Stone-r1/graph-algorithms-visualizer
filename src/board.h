#include <vector>
#include <set>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>

#include "raylib.h"
#include "node.h"
#include "traversalAlgorithm.h"

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

    std::vector<std::vector<std::pair<int, int>>> graph;
    std::vector<Node> nodes;
    std::set<std::pair<int, int>> highlightedEdges;
    std::unordered_map<int, int> highlightedWeights;

    std::unique_ptr<TraversalAlgorithm> currentAlgo;

    Node* findNodeFromPosition(const Vector2& firstNodePosition);
    bool runAlgorithm(const Vector2& mousePosition, bool weightRequired, std::function<std::unique_ptr<TraversalAlgorithm>(int)> algorithmConstructor);

public:
    Board();
    ~Board() = default;

    // =================
    // Disable copying
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    // Enable moving
    Board(Board&&) noexcept = default;
    Board& operator=(Board&&) noexcept = default;
    // Callers can transfer ownership but can't copy.
    // =================

    void clear();

    std::optional<Vector2> isInNodeDomain(const Vector2& mousePosition);
    bool isInBoardBorderDomain(const Vector2& mousePosition, float currentRadius, int screenWidth, int screenHeight);
    void addNode(const Vector2& mousePosition, float currentRadius);
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
    void highlightNode(int index);
    void highlightEdge(int from, int to);
    void highlightWeight(int to, int weight);
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
