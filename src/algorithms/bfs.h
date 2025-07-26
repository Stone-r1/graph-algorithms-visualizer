#ifndef BFS_H
#define BFS_H

#include "traversalAlgorithm.h"
#include <vector>
#include <queue>

using std::vector;
using std::pair;

class BFS : public TraversalAlgorithm {
private:
    const vector<vector<pair<int, int>>>& graph;
    std::queue<pair<int, int>> q;
    vector<bool> visited;
    bool finished;
    int currentStepIndex = -1;
    vector<pair<int, int>> history;
    int start;

public:
    BFS(const vector<vector<pair<int, int>>>& adj, int startNode);

    pair<int, int> stepForward();
    pair<int, int> stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    pair<int, int> getHistory(int index) const;
    int getStartNode() const;
};

#endif
