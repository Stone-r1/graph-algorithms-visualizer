#include <algorithm>
#include <cmath>

#include "board.h"
#include "node.h"
#include "data/constants.h"

#include "algorithms/BFS/bfs.h"
#include "algorithms/DFS/dfs.h"
#include "algorithms/Dijkstra/dijkstra.h"
#include "algorithms/Bellman-Ford/bellmanFord.h"

// TODO: Make general pattern for running algorithms
Board::Board() :
    edges(0),
    lastNodeIndex(0),
    currentStep(0),
    isDirected(false),
    isWeighted(false),
    isRunning(false),
    lastClickedNode(Node::makeInvalidNode()),
    graph(NodeConstants::MAX_NODES),
    nodes(NodeConstants::MAX_NODES, Node::makeInvalidNode())
{
    graph.reserve(NodeConstants::MAX_NODES);
    nodes.reserve(NodeConstants::MAX_NODES);
}

void Board::clear() {
    resetRunning();
    graph.clear();
    nodes.clear();

    graph.resize(NodeConstants::MAX_NODES);
    nodes.assign(NodeConstants::MAX_NODES, Node::makeInvalidNode());
    edges = 0;

    if (isGraphWeighted()) {
        flipGraphWeight();
    }
}

Node* Board::findNodeFromPosition(const Vector2& firstNodePosition) {
    for (Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(firstNodePosition)) {
            return &node;
        }
    }
    return nullptr;
}

// general pattern for running algorithm
bool Board::runAlgorithm(const Vector2& mousePosition, bool weightRequired, std::function<std::unique_ptr<TraversalAlgorithm>(int)> algorithmConstructor) {
    if (weightRequired != isGraphWeighted()) {
        // graph must be weighted/not weighted to run | later for UI
        return false;
    } 

    Node* startNode = findNodeFromPosition(mousePosition);
    if (!startNode) {
        return false;
    }

    resetRunning();
    currentAlgo = algorithmConstructor(startNode -> getNodeIndex());
    isRunning = true;
    return true;
}

std::optional<Vector2> Board::isInNodeDomain(const Vector2& mousePosition) {
    for (const Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(mousePosition)) {
            return node.getNodePosition();
        }
    }

    return std::nullopt;
}

bool Board::isInBoardBorderDomain(const Vector2& mousePosition, float currentRadius, int screenWidth, int screenHeight) { 
    const float allowedMargin = UIConstants::MARGIN_FROM_BORDER * currentRadius;
    return (mousePosition.y <= allowedMargin ||
            mousePosition.y >= screenHeight - allowedMargin ||
            mousePosition.x <= allowedMargin ||
            mousePosition.x >= screenWidth - allowedMargin);
}

void Board::addNode(const Vector2& mousePosition, float currentRadius) {
    for (const Node& node : nodes) {
        float minDistance = node.getNodeRadius() * UIConstants::MARGIN_FROM_NODE;

        if (node.isNodeValid()) {
            Vector2 nodePosition = node.getNodePosition(); 

            float dx = mousePosition.x - nodePosition.x;
            float dy = mousePosition.y - nodePosition.y;

            if (dx * dx + dy * dy <= minDistance * minDistance) {
                return;
            }
        }
    }

    Node currentNode = Node(lastNodeIndex, mousePosition, currentRadius);
    nodes[lastNodeIndex++] = currentNode;
}

void Board::addEdge(const Vector2& firstNodePosition, const Vector2& secondNodePosition, int weight) {
    Node* firstNode = findNodeFromPosition(firstNodePosition);
    Node* secondNode = findNodeFromPosition(secondNodePosition);

    if (!firstNode || !secondNode) {
        return;
    }

    edges++;
    int firstNodeIndex = firstNode->getNodeIndex();
    int secondNodeIndex = secondNode->getNodeIndex();

    graph[firstNodeIndex].push_back({secondNodeIndex, weight}); 
    firstNode->addNeighbor(secondNodeIndex);

    graph[secondNodeIndex].push_back({firstNodeIndex, weight});
    secondNode->addNeighbor(firstNodeIndex);

    printf("Current amount of edges is: %d\n", edges);
}

void Board::removeEdge(const Vector2& firstNodePosition, const Vector2& secondNodePosition) {
    Node* firstNode = findNodeFromPosition(firstNodePosition);
    Node* secondNode = findNodeFromPosition(secondNodePosition);

    if (!firstNode || !secondNode) {
        return;
    }

    edges--;
    int firstNodeIndex = firstNode->getNodeIndex();
    int secondNodeIndex = secondNode->getNodeIndex();

    auto& neighbors1 = graph[firstNodeIndex];
    neighbors1.erase(std::remove_if(neighbors1.begin(), neighbors1.end(), 
                [secondNodeIndex](const std::pair<int, int>& p) {return p.first == secondNodeIndex;
                }), neighbors1.end());
    firstNode->removeNeighbor(secondNodeIndex);
   
    auto& neighbors2 = graph[secondNodeIndex];
    neighbors2.erase(std::remove_if(neighbors2.begin(), neighbors2.end(),
                [firstNodeIndex](const std::pair<int, int>& p) {return p.first == firstNodeIndex;
                }), neighbors2.end());
    secondNode->removeNeighbor(firstNodeIndex);

    printf("Current amount of edges is: %d\n", edges);
}

void Board::removeNode(const Vector2& mousePosition) {
    Node* nodeToRemove = findNodeFromPosition(mousePosition);
    if (!nodeToRemove) return;

    edges -= graph[nodeToRemove->getNodeIndex()].size();
    graph[nodeToRemove->getNodeIndex()].clear();
    nodeToRemove->removeNeighbors();

    for (int i = 0; i < lastNodeIndex; i++) {
        if (i == nodeToRemove->getNodeIndex()) continue;

        auto& adj = graph[i];
        adj.erase(std::remove_if(adj.begin(), adj.end(),
                    [nodeToRemove](const std::pair<int, int>& p) {return p.first == nodeToRemove->getNodeIndex();
                    }), adj.end());
        nodes[i].removeNeighbor(nodeToRemove->getNodeIndex());
    }

    // mark node as invalid
    *nodeToRemove = Node::makeInvalidNode();
    printf("Current amount of edges is: %d\n", edges);
}

void Board::drawNodes() {
    using namespace ColorConstants;
    int startIndex = currentAlgo ? currentAlgo->getStartNode() : startNodeIndex;

    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        Vector2 nodePosition = node.getNodePosition();
        Color color = NODE_DEFAULT; 
        if (node.getNodeIndex() == startNodeIndex) {
            color = NODE_START;
        } else if (node.highlighted()) {
            color = currentStep == -1 ? NODE_START : NODE_VISITED;
        }

        DrawCircle(nodePosition.x, nodePosition.y, node.getNodeRadius(), color);
        DrawRing(nodePosition, node.getNodeRadius() - node.getNodeRadius() / 8, node.getNodeRadius(), 0.0f, 360.0f, 100, BLACK);
    }  
}

void Board::drawEdges() {
    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        std::unordered_set<int> neighbors = node.getNodeNeighbors();
        for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
            Node* neighbor = &nodes[*it];
            if (!neighbor->isNodeValid()) continue;

            Color color = highlightedEdges.count({node.getNodeIndex(), neighbor->getNodeIndex()}) ? ColorConstants::EDGE_HIGHLIGHT : ColorConstants::EDGE_DEFAULT;
            DrawLineEx(node.getNodePosition(), neighbor->getNodePosition(), 6.0f, color);
        }
    }         
}

void Board::drawWeights(const Font& font) {
    for (auto& [nodeIndex, weight] : highlightedWeights) {

        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", weight);
        
        Vector2 positions = nodes[nodeIndex].getNodePosition();
        Vector2 textSize = MeasureTextEx(font, buffer, UIConstants::FONT_SIZE_NODE, 1);
        Vector2 drawPos = {
            positions.x - textSize.x / 2,
            positions.y - textSize.y / 2
        };

        DrawTextEx(font, buffer, drawPos, UIConstants::FONT_SIZE_NODE, 1, ColorConstants::TEXT);
    }

    for (Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        std::unordered_set<int> neighbors = node.getNodeNeighbors();
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            if (!nodes[*it].isNodeValid()) continue;
            if (!isDirected && node.getNodeIndex() > *it) continue;

            bool found = false;
            int weight = 0;
            for (const auto& [neighbor, w] : graph[node.getNodeIndex()]) {
                if (neighbor == *it) {
                    weight = w;
                    found = true;
                    break;
                }
            }

            if (!found) continue;

            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%d", weight);

            Vector2 posFrom = node.getNodePosition();
            Vector2 posTo = nodes[*it].getNodePosition();

            // entire point is to avoid weight and edge intersection.
            float midX = (posFrom.x + posTo.x) / 2;
            float midY = (posFrom.y + posTo.y) / 2;

            float dx = posTo.x - posFrom.x;
            float dy = posTo.y - posFrom.y;

            float length = sqrtf(dx * dx + dy * dy);
            if (length == 0) length = 1; 

            // Perpendicular vector (normal)
            float nx = -dy / length;
            float ny = dx / length;

            midX += nx * UIConstants::WEIGHT_OFFSET;
            midY += ny * UIConstants::WEIGHT_OFFSET;

            DrawTextEx(font, buffer, {midX - 10, midY - 10}, UIConstants::FONT_SIZE_EDGE, 1, ColorConstants::TEXT);
            // ==========================
        }
    }
}

void Board::resetRunning() {
    currentAlgo.reset();
    resetHighlights();
 
    currentStep = 0;
    weightDigitCount = 0;
    isRunning = false;
    weightReady = false;
    isEnteringWeight = false;
    startNodeIndex = -1;
    std::fill(std::begin(weightInput), std::end(weightInput), '\0');
}

bool Board::isAlgorithmRunning() const {
    return isRunning;
}

bool Board::runBFS(const Vector2& startNodePosition) {
    return runAlgorithm(startNodePosition, false, [this](int startNodeIndex){return std::make_unique<BFS>(graph, startNodeIndex);});
}

bool Board::runDFS(const Vector2& startNodePosition) {
    return runAlgorithm(startNodePosition, false, [this](int startNodeIndex){return std::make_unique<DFS>(graph, startNodeIndex);});
}

bool Board::runDijkstra(const Vector2& startNodePosition) {
    return runAlgorithm(startNodePosition, true, [this](int startNodeIndex){return std::make_unique<Dijkstra>(graph, startNodeIndex);});
}

bool Board::runBellmanFord(const Vector2& startNodePosition) {
    return runAlgorithm(startNodePosition, true, [this](int startNodeIndex){return std::make_unique<BellmanFord>(graph, startNodeIndex);});
}

void Board::highlightNode(int index) {
    if (index >= 0 && index < nodes.size() && nodes[index].isNodeValid()) {
        nodes[index].setHighlight();
    }
}

void Board::highlightEdge(int from, int to) {
      highlightedEdges.insert({from, to});
      if (!isDirected && from != to) {
          highlightedEdges.insert({to, from});
      }
}

void Board::highlightWeight(int to, int weight) {
    highlightedWeights[to] = weight; 
}

void Board::highlightStartingNode(const Vector2& mousePosition) {
    Node* startNode = findNodeFromPosition(mousePosition);
    if (!startNode) {
        return;
    }

    startNodeIndex = startNode->getNodeIndex(); 

    for (Node& node : nodes) {
        node.resetHighlight();
    }

    nodes[startNodeIndex].setHighlight();
}

void Board::resetHighlights() {
    for (Node& node : nodes) {
        node.resetHighlight();
    }

    highlightedEdges.clear();
    highlightedWeights.clear();
}

void Board::stepForward() {
    if (currentAlgo && !currentAlgo->isFinished()) {
        auto step = currentAlgo->stepForward();
        if (step.isValid()) { 
            auto [from, to, weight] = step;

            if (isGraphWeighted()) {
                highlightWeight(to, weight);
            }

            highlightEdge(from, to);
            highlightNode(to);
        }
    }
}

void Board::stepBackward() {
    if (currentAlgo && currentAlgo->getCurrentStepIndex() >= 0) {
        auto [parent, node, weight] = currentAlgo->stepBackward();
        resetHighlights();
        for (int i = 0; i <= currentAlgo->getCurrentStepIndex(); i++) {
            auto [from, to, weight] = currentAlgo->getHistory(i); 
            if (isGraphWeighted()) {
                highlightWeight(to, weight);
            }

            highlightEdge(from, to);
            highlightNode(to); 
        }
    }
}

// related to graph weights

bool Board::isGraphWeighted() const {
    return isWeighted;
}

bool Board::isGraphEmpty() const {
    return edges == 0;
}

void Board::flipGraphWeight() {
    isWeighted = !isWeighted;
}

void Board::askForWeight() {
    Rectangle box = {300, 200, 200, 50};
    DrawRectangleRec(box, ColorConstants::WEIGHT_BOX_BACKGROUND);
    DrawRectangleLinesEx(box, 2, ColorConstants::WEIGHT_BOX_BORDER);
    DrawText(weightInput, box.x + 10, box.y + 15, 30, ColorConstants::TEXT);

    int key = GetCharPressed();
    while (key > 0) {
        if (isdigit(key) && weightDigitCount < 9) {
            weightInput[weightDigitCount++] = (char)key;
            weightInput[weightDigitCount] = '\0';
        } else if (key == '-' && !weightDigitCount) {
            weightInput[weightDigitCount++] = '-';
            weightInput[weightDigitCount] = '\0';
        }

        key = GetCharPressed();
    }
 
    if (IsKeyPressed(KEY_BACKSPACE) && weightDigitCount > 0) {
        weightInput[--weightDigitCount] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER)) {
        weightReady = true;
        isEnteringWeight = true;
    }
}

int Board::getCurrentWeight() {
    int result = atoi(weightInput);
    weightInput[0] = '\0';
    weightDigitCount = 0;
    weightReady = true;
    return result;
}

bool Board::isWeightReady() const {
    return weightReady;
}

void Board::setWeightReady() {
    weightReady = !weightReady;
}
