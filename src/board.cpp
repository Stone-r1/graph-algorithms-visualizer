#include <vector>
#include <set>
#include <algorithm>

#include "raylib.h"
#include "board.h"
#include "node.h"
#include "algorithms/bfs.h"

#define MAX_NODES 1000

Board::Board() :
    edges(0),
    lastNodeIndex(0),
    currentStep(0),
    isDirected(false),
    isRunning(false),
    lastClickedNode(-1, {0, 0}, 0),
    graph(MAX_NODES),
    nodes(MAX_NODES, Node(-1, {0, 0}, 0)),
    traversalOrder(MAX_NODES)
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

void Board::addEdge(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    Node* firstNode = findNodeFromPosition(firstNodePosition);
    Node* secondNode = findNodeFromPosition(secondNodePosition);

    if (!firstNode || !secondNode) {
        return;
    }

    int firstNodeIndex = firstNode->getNodeIndex();
    int secondNodeIndex = secondNode->getNodeIndex();

    graph[firstNodeIndex].push_back(secondNodeIndex); 
    firstNode->addNeighbor(secondNodeIndex);

    if (!isDirected) {
        graph[secondNodeIndex].push_back(firstNodeIndex);
        secondNode->addNeighbor(firstNodeIndex);
    }
}

void Board::removeEdge(Vector2 firstNodePosition, Vector2 secondNodePosition) {
    Node* firstNode = findNodeFromPosition(firstNodePosition);
    Node* secondNode = findNodeFromPosition(secondNodePosition);

    if (!firstNode || !secondNode) {
        return;
    }

    int firstNodeIndex = firstNode->getNodeIndex();
    int secondNodeIndex = secondNode->getNodeIndex();

    auto& neighbors1 = graph[firstNodeIndex];
    neighbors1.erase(std::remove(neighbors1.begin(), neighbors1.end(), secondNodeIndex), neighbors1.end());
    firstNode->removeNeighbor(secondNodeIndex);
   
    if (!isDirected) {
        auto& neighbors2 = graph[secondNodeIndex];
        neighbors2.erase(std::remove(neighbors2.begin(), neighbors2.end(), firstNodeIndex), neighbors2.end());
        secondNode->removeNeighbor(firstNodeIndex);
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
    bfs.reset();
    resetHighlights();
}

void Board::runBFS(Vector2 startNodePosition) {
    Node* startNode = findNodeFromPosition(startNodePosition);
    if (startNode) {
        bfs = std::make_unique<BFS>(graph, startNode->getNodeIndex());
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

void Board::stepForwardBFS() {
    if (bfs && !bfs->isFinished()) {
        auto [from, to] = bfs->stepForward();
        if (from != -1) {
            highlightEdge(from, to);
            highlightNode(to);
        }
    }
}

void Board::stepBackwardBFS() {
    if (bfs && bfs->getCurrentStepIndex() >= 0) {
        auto [parent, node] = bfs->stepBackward();
        resetHighlights();
        for (int i = 0; i <= bfs->getCurrentStepIndex(); i++) {
            auto [from, to] = bfs->getHistory(i);
            highlightEdge(from, to);
            highlightNode(to);
        }
    }
}
