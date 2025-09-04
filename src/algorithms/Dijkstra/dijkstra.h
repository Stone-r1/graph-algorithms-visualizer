#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "traversalAlgorithm.h"
#include <vector>
#include <queue>

class Dijkstra : public TraversalAlgorithm {
private:
    const std::vector<std::vector<std::pair<int, int>>>& graph;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    std::vector<long long> distances;
    std::vector<bool> visited;
    bool finished;
    bool precompiled = false;
    int currentStepIndex = -1;
    std::vector<Step> history;
    int start;

public:
    Dijkstra(const std::vector<std::vector<std::pair<int, int>>>& adj, int startNode);

    Step stepForward();
    Step stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    Step getHistory(int index) const;
    int getStartNode() const;
    int getCumulativeDistance(int nodeIndex) const;
};

#endif
