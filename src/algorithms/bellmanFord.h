#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "traversalAlgorithm.h"
#include <vector>

using std::vector;
using std::pair;

class BellmanFord : public TraversalAlgorithm {
private:
    const vector<vector<pair<int, int>>>& graph;
    vector<long long> distances;
    bool finished;
    int currentStepIndex = -1;
    vector<Step> history;
    int start;

public:
    BellmanFord(const vector<vector<pair<int, int>>>& adj, int startNode);

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
