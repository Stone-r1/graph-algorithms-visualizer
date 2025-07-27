#include "dijkstra.h"
const long long MAX_VALUE = 1e18;

Dijkstra::Dijkstra(const vector<vector<pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    distances(adj.size(), MAX_VALUE),
    visited(adj.size(), false),
    finished(false),
    start(startNode)
{
    distances[start] = 0;
    pq.emplace(0, startNode);
    history.push_back({start, start});
    visited[startNode] = true;
}

Step Dijkstra::stepForward() {
    if (finished) {
        return {-1, -1, -1};
    } 
    
    if (currentStepIndex + 1 < static_cast<int>(history.size())) {
        return history[++currentStepIndex];
    }

    while (!pq.empty()) {
        auto [distance, node] = pq.top();
        pq.pop();

        if (visited[node]) continue;
        visited[node] = true;

        Step step = {node, node, distances[node]};
        history.push_back(step);
        currentStepIndex++;

        for (auto& [neighbor, weight] : graph[node]) {
            if (visited[neighbor] || distances[neighbor] <= distances[node] + weight) {
                continue;
            }

            distances[neighbor] = distances[node] + weight;
            pq.emplace(distances[neighbor], neighbor);

            Step relaxStep = {node, neighbor, distances[neighbor]};
            history.push_back(relaxStep);
        }

        return step;
    }

    finished = true;
    return {-1, -1, -1};
}

Step Dijkstra::stepBackward() {
    if (currentStepIndex < 0) {
        return {-1, -1, -1};
    }
    
    return history[currentStepIndex--]; 
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
