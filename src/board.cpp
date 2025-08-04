#include <vector>
#include <set>
#include <algorithm>
#include <ctype.h>
#include <iostream>
#include <cmath>

#include "raylib.h"
#include "board.h"
#include "node.h"

#define MAX_NODES 1000

Board::Board() :
    edges(0),
    lastNodeIndex(0),
    currentStep(0),
    isDirected(false),
    isWeighted(false),
    isRunning(false),
    lastClickedNode(Node::makeInvalidNode()),
    graph(MAX_NODES),
    nodes(MAX_NODES, Node::makeInvalidNode())
{}

void Board::clear() {
    resetRunning();
    graph.clear();
    nodes.clear();

    graph.resize(MAX_NODES);
    nodes.assign(MAX_NODES, Node::makeInvalidNode());
}

Node* Board::findNodeFromPosition(const Vector2& firstNodePosition) {
    for (Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(firstNodePosition)) {
            return &node;
        }
    }
    return nullptr;
}

Vector2 Board::isInNodeDomain(const Vector2& mousePosition) {
    for (const Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(mousePosition)) {
            return node.getNodePosition();
        }
    }

    return {0.0f, 0.0f};
}

void Board::addNode(const Vector2& mousePosition, const float& currentRadius) {
    for (const Node& node : nodes) {
        float minDistance = node.getNodeRadius() * 3;

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

    if (!isDirected) {
        graph[secondNodeIndex].push_back({firstNodeIndex, weight});
        secondNode->addNeighbor(firstNodeIndex);
        edges++;
    }
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
                [secondNodeIndex](const pair<int, int>& p) {return p.first == secondNodeIndex;
                }), neighbors1.end());
    firstNode->removeNeighbor(secondNodeIndex);
   
    if (!isDirected) {
        auto& neighbors2 = graph[secondNodeIndex];
        neighbors2.erase(std::remove_if(neighbors2.begin(), neighbors2.end(),
                    [firstNodeIndex](const pair<int, int>& p) {return p.first == firstNodeIndex;
                    }), neighbors2.end());
        secondNode->removeNeighbor(firstNodeIndex);
        edges--;
    }
}

void Board::removeNode(const Vector2& mousePosition) {
    Node* nodeToRemove = findNodeFromPosition(mousePosition);
    if (!nodeToRemove) return;

    graph[nodeToRemove->getNodeIndex()].clear();
    nodeToRemove->removeNeighbors();

    for (int i = 0; i < lastNodeIndex; i++) {
        if (i == nodeToRemove->getNodeIndex()) continue;

        auto& adj = graph[i];
        adj.erase(std::remove_if(adj.begin(), adj.end(),
                    [nodeToRemove](const pair<int, int>& p) {return p.first == nodeToRemove->getNodeIndex();
                    }), adj.end());
        nodes[i].removeNeighbor(nodeToRemove->getNodeIndex());
    }

    // mark node as invalid
    *nodeToRemove = Node::makeInvalidNode();
}

void Board::drawNodes() {
    int startIndex = currentAlgo ? currentAlgo->getStartNode() : startNodeIndex;

    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        Vector2 nodePosition = node.getNodePosition();
        Color color = BLUE; // standard
        if (node.getNodeIndex() == startNodeIndex) {
            color = ORANGE;
        } else if (node.highlighted()) {
            color = currentStep == -1 ? ORANGE : RED;
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

            Color color = highlightedEdges.count({node.getNodeIndex(), neighbor->getNodeIndex()}) ? DARKBLUE : GREEN;
            DrawLineEx(node.getNodePosition(), neighbor->getNodePosition(), 6.0f, color);
            // DrawLineEx(node.getNodePosition(), neighbor->getNodePosition(), 2.0f, color);
        }
    }         
}

void Board::drawWeights() {
    for (auto& [nodeIndex, weight] : highlightedWeights) {

        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", weight);
        
        Vector2 positions = nodes[nodeIndex].getNodePosition();
        DrawText(buffer, positions.x - 10, positions.y - 10, 30, BLACK);
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

            float offset = 20.0f;

            midX += nx * offset;
            midY += ny * offset;

            DrawText(buffer, midX - 10, midY - 10, 30, BLACK);
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
    std::fill(std::begin(weightInput), std::end(weightInput), '\0');
}

void Board::runBFS(const Vector2& startNodePosition) {
    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        resetRunning();
        startNodeIndex = -1;
        currentAlgo = std::make_unique<BFS>(graph, startNode->getNodeIndex());
        isRunning = true;
    }
}

void Board::runDFS(const Vector2& startNodePosition) {
    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        resetRunning();
        startNodeIndex = -1;
        currentAlgo = std::make_unique<DFS>(graph, startNode->getNodeIndex());
        isRunning = true;
    }
}

void Board::runDijkstra(const Vector2& startNodePosition) {
    if (!isGraphWeighted()) {
        std::cout << "Graph must be weighted to run dijkstra\n";
        return;
    }

    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        resetRunning();
        startNodeIndex = -1;
        currentAlgo = std::make_unique<Dijkstra>(graph, startNode->getNodeIndex());
        isRunning = true;
    }
}

void Board::runBellmanFord(const Vector2& startNodePosition) {
    if (!isGraphWeighted()) {
        std::cout << "Graph must be weighted to run dijkstra\n";
        return;
    }

    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        resetRunning();
        startNodeIndex = -1;
        currentAlgo = std::make_unique<BellmanFord>(graph, startNode->getNodeIndex());
        isRunning = true;
    }
}

void Board::highlightNode(const int& index) {
    if (index >= 0 && index < nodes.size() && nodes[index].isNodeValid()) {
        nodes[index].setHighlight();
    }
}

void Board::highlightEdge(const int& from, const int& to) {
      highlightedEdges.insert({from, to});
      if (!isDirected && from != to) {
          highlightedEdges.insert({to, from});
      }
}

void Board::highlightWeight(const int& to, const int& weight) {
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
        auto [from, to, weight] = currentAlgo->stepForward();
        if (from != -1) { 
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
    Color color = {254, 248, 221, 255};
    DrawRectangleRec(box, color);
    DrawRectangleLinesEx(box, 2, BLACK);
    DrawText(weightInput, box.x + 10, box.y + 15, 30, DARKGRAY);
    int key = GetCharPressed();

    while (key > 0) {
        if (isdigit(key) && weightDigitCount < 9) {
            weightInput[weightDigitCount++] = (char)key;
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
