#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "traversalAlgorithm.h"
#include <vector>
#include <queue>
#include <tuple>

using std::vector;
using std::pair;

class Dijkstra : public TraversalAlgorithm {
private:
    const vector<vector<pair<int, int>>>& graph;
    std::priority_queue<pair<int, int>, vector<pair<int, int>>, std::greater<>> pq;
    std::vector<long long> distances;
    vector<bool> visited;
    bool finished;
    bool precompiled = false;
    int currentStepIndex = -1;
    vector<Step> history;
    int start;

public:
    Dijkstra(const vector<vector<pair<int, int>>>& adj, int startNode);

    Step stepForward();
    Step stepBackward();

    bool isFinished() const;
    int getCurrentStepIndex() const;
    int getTotalSteps() const;
    Step getHistory(int index) const;
    int getStartNode() const;
};

#endif
