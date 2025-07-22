#include "dfs.h"

DFS::DFS(const vector<vector<int>>& adj, int startNode) : 
    graph(adj),
    visited(adj.size(), false),
    finished(false)
{
    st.push({startNode, startNode});
    visited[startNode] = true;
}

pair<int, int> DFS::stepForward() {
    if (finished) {
        return {-1, -1};
    } 
    
    if (currentStepIndex + 1 < history.size()) {
        return history[++currentStepIndex];
    }

    if (st.empty()) {
        finished = true;
        return {-1, -1};
    }

    auto [parent, node] = st.top();
    visited[node] = true;
    st.pop();

    for (auto it = graph[node].rbegin(); it != graph[node].rend(); it++) {
        if (!visited[*it]) {
            st.push({node, *it});
        }
    }
    
    currentStepIndex++;
    history.push_back({parent, node});
    return {parent, node};
}

pair<int, int> DFS::stepBackward() {
    if (currentStepIndex < 0) {
        return {-1, -1};
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

pair<int, int> DFS::getHistory(int index) const {
    return history[index];
}

