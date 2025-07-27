#include "dfs.h"

DFS::DFS(const vector<vector<pair<int, int>>>& adj, int startNode) : 
    graph(adj),
    visited(adj.size(), false),
    finished(false),
    start(startNode)
{
    st.push({startNode, startNode});
    visited[startNode] = true;
}

Step DFS::stepForward() {
    if (finished) {
        return {-1, -1, -1};
    } 
    
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (st.empty()) {
        finished = true;
        return {-1, -1, -1};
    }

    auto [parent, node] = st.top();
    visited[node] = true;
    st.pop();

    for (auto it = graph[node].rbegin(); it != graph[node].rend(); it++) {
        if (!visited[(*it).first]) {
            st.push({node, (*it).first});
        }
    }
    
    currentStepIndex++;
    Step step = {parent, node, 1};
    history.push_back(step);
    return step;
}

Step DFS::stepBackward() {
    if (currentStepIndex < 0) {
        return {-1, -1, -1};
    }
    
    return history[currentStepIndex--]; 
}

bool DFS::isFinished() const {
    return finished && currentStepIndex + 1 >= history.size();
}

int DFS::getCurrentStepIndex() const {
    return currentStepIndex;
}

int DFS::getTotalSteps() const {
    return history.size();
}

Step DFS::getHistory(int index) const {
    return history[index];
}

int DFS::getStartNode() const {
    return start;
}
