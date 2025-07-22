#ifndef BFS_H
#define BFS_H

#include <vector>
#include <queue>

using std::vector;
using std::pair;

class BFS {
private:
    const vector<vector<int>>& graph;
    std::queue<pair<int, int>> q;
    vector<bool> visited;
    bool finished;
    int currentStepIndex = -1;
    vector<pair<int, int>> history;

public:
    BFS(const vector<vector<int>>& adj, int startNode);

    pair<int, int> stepForward();
    pair<int, int> stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    pair<int, int> getHistory(int index) const;
};

#endif
