#include "bfs.h"

BFS::BFS(const std::vector<std::vector<std::pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    visited(adj.size(), false),
    finished(false),
    start(startNode)
{
    q.push({startNode, startNode});
    visited[startNode] = true;
}

Step BFS::stepForward() {
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (finished) {
        return Step::invalidStep();
    } 

    if (q.empty()) {
        finished = true;
        return Step::invalidStep();
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
    Step step = {parent, node, 1};
    history.push_back(step);
    return step;
}

Step BFS::stepBackward() {
    if (currentStepIndex < 0) {
        return Step::invalidStep();
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

Step BFS::getHistory(int index) const {
    return history[index];
}

int BFS::getStartNode() const {
    return start;
}
