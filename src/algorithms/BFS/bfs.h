#ifndef BFS_H
#define BFS_H

#include "traversalAlgorithm.h"
#include <vector>
#include <queue>

class BFS : public TraversalAlgorithm {
private:
    const std::vector<std::vector<std::pair<int, int>>>& graph;
    std::queue<std::pair<int, int>> q;
    std::vector<bool> visited;
    bool finished;
    int currentStepIndex = -1;
    std::vector<Step> history;
    int start;

public:
    BFS(const std::vector<std::vector<std::pair<int, int>>>& adj, int startNode);

    Step stepForward();
    Step stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    Step getHistory(int index) const;
    int getStartNode() const;
};

#endif
