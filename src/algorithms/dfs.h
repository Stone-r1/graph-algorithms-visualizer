#ifndef DFS_H
#define DFS_H

#include "traversalAlgorithm.h"
#include <vector>
#include <stack>

using std::vector;
using std::pair;

class DFS : public TraversalAlgorithm {
private:
    const vector<vector<int>>& graph;
    std::stack<pair<int, int>> st;
    vector<bool> visited;
    bool finished;
    int currentStepIndex = -1;
    vector<pair<int, int>> history;

public:
    DFS(const vector<vector<int>>& adj, int startNode);

    pair<int, int> stepForward();
    pair<int, int> stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    pair<int, int> getHistory(int index) const;
};

#endif
