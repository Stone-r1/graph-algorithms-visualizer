#ifndef DFS_H
#define DFS_H

#include "traversalAlgorithm.h"
#include <vector>
#include <stack>

class DFS : public TraversalAlgorithm {
private:
    const std::vector<std::vector<std::pair<int, int>>>& graph;
    std::stack<std::pair<int, int>> st;
    std::vector<bool> visited;
    bool finished;
    int currentStepIndex = -1;
    std::vector<Step> history;
    int start;

public:
    DFS(const std::vector<std::vector<std::pair<int, int>>>& adj, int startNode);

    Step stepForward();
    Step stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    Step getHistory(int index) const;
    int getStartNode() const;
};

#endif
