#include "bfs.h"

BFS::BFS(const vector<vector<pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    visited(adj.size(), false),
    finished(false)
{
    q.push({startNode, startNode});
    visited[startNode] = true;
}

pair<int, int> BFS::stepForward() {
    if (finished) {
        return {-1, -1};
    } 
    
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (q.empty()) {
        finished = true;
        return {-1, -1};
    }

    auto [parent, node] = q.front();
    q.pop();

    for (auto& [neighbor, weight] : graph[node]) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push({node, neighbor});
        }
    }
    
    currentStepIndex++;
    history.push_back({parent, node});
    return {parent, node};
}

pair<int, int> BFS::stepBackward() {
    if (currentStepIndex < 0) {
        return {-1, -1};
    }
    
    return history[currentStepIndex--]; 
}

bool BFS::isFinished() const {
    return finished && currentStepIndex + 1 >= history.size();
}


int BFS::getCurrentStepIndex() const {
    return currentStepIndex;
}

int BFS::getTotalSteps() const {
    return history.size();
}

pair<int, int> BFS::getHistory(int index) const {
    return history[index];
}
