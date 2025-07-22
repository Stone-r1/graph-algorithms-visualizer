#ifndef BFS_H
#define BFS_H

#include <vector>
#include <queue>

class BFS {
private:
    const std::vector<std::vector<int>>& graph;
    std::queue<std::pair<int, int>> q;
    std::vector<bool> visited;
    bool finished;

public:
    BFS(const std::vector<std::vector<int>>& adj, int startNode);

    std::pair<int, int> step();
    bool isFinished() const;
};

#endif
