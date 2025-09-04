#include "dijkstra.h"
const long long MAX_VALUE = 1e18;

Dijkstra::Dijkstra(const std::vector<std::vector<std::pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    distances(adj.size(), MAX_VALUE),
    visited(adj.size(), false),
    finished(false),
    start(startNode)
{
    distances[startNode] = 0;
    pq.emplace(0, startNode);
    history.push_back({startNode, startNode, 0});
}

Step Dijkstra::stepForward() {  
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (finished) {
        return Step::invalidStep();
    }

    while (!pq.empty()) {
        auto [distance, node] = pq.top();
        pq.pop();

        if (visited[node]) continue;
        visited[node] = true;

        bool stepAdded = false;

        for (auto& [neighbor, weight] : graph[node]) {
            if (distances[neighbor] <= distance + weight) {
                continue;
            }

            distances[neighbor] = distance + weight;
            pq.emplace(distances[neighbor], neighbor);
            Step step = {node, neighbor, distances[neighbor]};
            history.push_back(step);
            stepAdded = true;
        }

        if (stepAdded) {
            return history[++currentStepIndex];
        }
    }

    finished = true;
    return Step::invalidStep();
}

Step Dijkstra::stepBackward() {
    if (currentStepIndex <= 0) {
        return Step::invalidStep();
    }
    
    return history[--currentStepIndex]; 
}

bool Dijkstra::isFinished() const {
    return finished && currentStepIndex + 1 >= history.size();
}

int Dijkstra::getCurrentStepIndex() const {
    return currentStepIndex;
}

int Dijkstra::getTotalSteps() const {
    return history.size();
}

Step Dijkstra::getHistory(int index) const {
    return history[index];
}

int Dijkstra::getStartNode() const {
    return start;
}

int Dijkstra::getCumulativeDistance(int nodeIndex) const {
    return distances[nodeIndex];
}
