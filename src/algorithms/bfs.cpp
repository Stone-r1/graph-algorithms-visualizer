#include "bfs.h"
#include <vector>
#include <queue>

BFS::BFS(const std::vector<std::vector<int>>& adj, int startNode) : 
    graph(adj),
    visited(adj.size(), false),
    finished(false)
{
    q.push({startNode, startNode});
    visited[startNode] = true;
}

std::pair<int, int> BFS::step() {
    if (q.empty()) {
        finished = true;
        return {-1, -1};
    }

    auto [from, node] = q.front();
    q.pop();

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            q.push({node, neighbor});
        }
    }

    return {from, node};
}

bool BFS::isFinished() const {
    return finished;
}
