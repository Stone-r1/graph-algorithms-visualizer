#ifndef DFS_H
#define DFS_H

#include "traversalAlgorithm.h"
#include <vector>
#include <stack>

using std::vector;
using std::pair;

class DFS : public TraversalAlgorithm {
private:
    const vector<vector<pair<int, int>>>& graph;
    std::stack<pair<int, int>> st;
    vector<bool> visited;
    bool finished;
    int currentStepIndex = -1;
    vector<Step> history;
    int start;

public:
    DFS(const vector<vector<pair<int, int>>>& adj, int startNode);

    Step stepForward();
    Step stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    Step getHistory(int index) const;
    int getStartNode() const;
};

#endif
