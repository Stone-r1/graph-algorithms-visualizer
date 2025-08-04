#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "traversalAlgorithm.h"
#include <vector>
#include <tuple>

using std::vector;
using std::pair;

class BellmanFord : public TraversalAlgorithm {
private:
    const vector<vector<pair<int, int>>>& graph;
    vector<std::tuple<int, int, int>> edges;
    vector<long long> distances;
    bool finished;
    vector<Step> history;

    int currentStepIndex = -1;
    int start;
    int vertices;
    int currentPhase;
    int currentEdgeIndex;

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
