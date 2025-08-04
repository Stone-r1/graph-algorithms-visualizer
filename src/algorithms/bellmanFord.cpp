#include "bellmanFord.h"
const long long MAX_VALUE = 1e18;

BellmanFord::BellmanFord(const vector<vector<pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    distances(adj.size(), MAX_VALUE),
    finished(false),
    start(startNode)
{
    distances[startNode] = 0;
    history.push_back({startNode, startNode, 0});
}

Step BellmanFord::stepForward() {  
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (finished) {
        return {-1, -1, -1};
    }

    return {-1, -1, -1};
}

Step BellmanFord::stepBackward() {
    if (currentStepIndex <= 0) {
        return {-1, -1, -1};
    }
    
    return history[--currentStepIndex]; 
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
    return history[index];
}

int BellmanFord::getStartNode() const {
    return start;
}

int BellmanFord::getCumulativeDistance(int nodeIndex) const {
    return distances[nodeIndex];
}
