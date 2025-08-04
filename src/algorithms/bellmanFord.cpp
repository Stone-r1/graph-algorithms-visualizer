#include "bellmanFord.h"
const long long MAX_VALUE = 1e18;

BellmanFord::BellmanFord(const vector<vector<pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    distances(adj.size(), MAX_VALUE),
    finished(false),
    start(startNode)
{
    vertices = graph.size();
    distances[startNode] = 0;
    history.push_back({startNode, startNode, 0, true});

    // flatten edges
    for (int node = 0; node < vertices; node++) {
        for (const auto& [neighbor, weight] : graph[node]) {
            edges.emplace_back(node, neighbor, weight);
        }
    }
}

Step BellmanFord::stepForward() {  
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (currentPhase >= vertices - 1) {
        finished = true;
        return {-1, -1, -1};
    }

    if (finished) {
        return {-1, -1, -1};
    } 

    auto [node, neighbor, weight] = edges[currentEdgeIndex];
    bool relaxed = false;
    if (distances[node] != MAX_VALUE && distances[node] + weight < distances[neighbor]) {
        distances[neighbor] = distances[node] + weight;
        relaxed = true;
    }

    if (++currentEdgeIndex >= edges.size()) {
        currentEdgeIndex = 0;
        currentPhase++;
    }

    if (currentPhase >= vertices - 1) {
        finished = true;
    }

    if (relaxed) { 
        BellmanStep step = BellmanStep({node, neighbor, distances[neighbor], relaxed});
        history.push_back(step);
        currentStepIndex++;
        return step.toStep();
    } else {
        return stepForward();
    }
}

Step BellmanFord::stepBackward() {
    if (currentStepIndex <= 0) {
        return {-1, -1, -1};
    }
    
    return history[--currentStepIndex].toStep();
}

bool BellmanFord::isFinished() const {
    return finished && currentStepIndex + 1 >= history.size();
}

int BellmanFord::getCurrentStepIndex() const {
    return currentStepIndex;
}

int BellmanFord::getTotalSteps() const {
    return history.size();
}

Step BellmanFord::getHistory(int index) const {
    return history[index].toStep();
}

int BellmanFord::getStartNode() const {
    return start;
}

int BellmanFord::getCumulativeDistance(int nodeIndex) const {
    return distances[nodeIndex];
}
