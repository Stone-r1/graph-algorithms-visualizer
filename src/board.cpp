#include <vector>
#include <set>
#include <algorithm>
#include <ctype.h>

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
    lastClickedNode(-1, {0, 0}, 0),
    graph(MAX_NODES),
    nodes(MAX_NODES, Node(-1, {0, 0}, 0))
{}

Node* Board::findNodeFromPosition(Vector2 firstNodePosition) {
    for (Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(firstNodePosition)) {
            return &node;
        }
    }
    return nullptr;
}

Vector2 Board::isInNodeDomain(Vector2 mousePosition) {
    for (const Node& node : nodes) {
        if (node.isNodeValid() && node.isInRadiusDomain(mousePosition)) {
            return node.getNodePosition();
        }
    }

    return {0.0f, 0.0f};
}

void Board::addNode(Vector2 mousePosition, float currentRadius) {
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

void Board::addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition, int weight) {
    Node* firstNode = findNodeFromPosition(firstNodePosition);
    Node* secondNode = findNodeFromPosition(secondNodePosition);

    if (!firstNode || !secondNode) {
        return;
    }

    edges++;
    int firstNodeIndex = firstNode->getNodeIndex();
    int secondNodeIndex = secondNode->getNodeIndex();

    graph[firstNodeIndex].push_back(secondNodeIndex); 
    firstNode->addNeighbor(secondNodeIndex);

    if (!isDirected) {
        graph[secondNodeIndex].push_back(firstNodeIndex);
        secondNode->addNeighbor(firstNodeIndex);
        edges++;
    }
}

void Board::removeEdge(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    Node* firstNode = findNodeFromPosition(firstNodePosition);
    Node* secondNode = findNodeFromPosition(secondNodePosition);

    if (!firstNode || !secondNode) {
        return;
    }

    edges--;
    int firstNodeIndex = firstNode->getNodeIndex();
    int secondNodeIndex = secondNode->getNodeIndex();

    auto& neighbors1 = graph[firstNodeIndex];
    neighbors1.erase(std::remove(neighbors1.begin(), neighbors1.end(), secondNodeIndex), neighbors1.end());
    firstNode->removeNeighbor(secondNodeIndex);
   
    if (!isDirected) {
        auto& neighbors2 = graph[secondNodeIndex];
        neighbors2.erase(std::remove(neighbors2.begin(), neighbors2.end(), firstNodeIndex), neighbors2.end());
        secondNode->removeNeighbor(firstNodeIndex);
        edges--;
    }
}

void Board::removeNode(Vector2 mousePosition) {
    Node* nodeToRemove = findNodeFromPosition(mousePosition);
    if (!nodeToRemove) return;

    graph[nodeToRemove->getNodeIndex()].clear();
    nodeToRemove->removeNeighbors();

    for (int i = 0; i < lastNodeIndex; i++) {
        if (i == nodeToRemove->getNodeIndex()) continue;

        auto& adj = graph[i];
        adj.erase(std::remove(adj.begin(), adj.end(), nodeToRemove->getNodeIndex()), adj.end());
        nodes[i].removeNeighbor(nodeToRemove->getNodeIndex());
    }

    // mark node as invalid
    *nodeToRemove = Node(-1, {0.0f, 0.0f}, 0.0f);
}

void Board::drawNodes() {
    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        Vector2 nodePosition = node.getNodePosition();
        Color color = node.highlighted() ? RED : BLUE;
        DrawCircle(nodePosition.x, nodePosition.y, node.getNodeRadius(), color);
    }  
}

void Board::drawEdges() {
    for (const Node& node : nodes) {
        if (!node.isNodeValid()) continue;

        std::set<int> neighbors = node.getNodeNeighbors();
        for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
            Node* neighbor = &nodes[*it];
            if (!neighbor->isNodeValid()) continue;

            Color color = highlightedEdges.count({node.getNodeIndex(), neighbor->getNodeIndex()}) ? DARKBLUE : GREEN;
            DrawLineEx(node.getNodePosition(), neighbor->getNodePosition(), 5.0f, color);
        }
    }         
}

void Board::resetRunning() {
    currentAlgo.reset();
    resetHighlights();
}

void Board::runBFS(Vector2 startNodePosition) {
    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        currentAlgo = std::make_unique<BFS>(graph, startNode->getNodeIndex());
    }
}

void Board::runDFS(Vector2 startNodePosition) {
    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        currentAlgo = std::make_unique<DFS>(graph, startNode->getNodeIndex());
    }
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

void Board::resetHighlights() {
    for (Node& node : nodes) {
        node.resetHighlight();
    }

    highlightedEdges.clear();
}

void Board::stepForward() {
    if (currentAlgo && !currentAlgo->isFinished()) {
        auto [from, to] = currentAlgo->stepForward();
        if (from != -1) {
            highlightEdge(from, to);
            highlightNode(to);
        }
    }
}

void Board::stepBackward() {
    if (currentAlgo && currentAlgo->getCurrentStepIndex() >= 0) {
        auto [parent, node] = currentAlgo->stepBackward();
        resetHighlights();
        for (int i = 0; i <= currentAlgo->getCurrentStepIndex(); i++) {
            auto [from, to] = currentAlgo->getHistory(i);
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
