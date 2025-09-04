#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "traversalAlgorithm.h"
#include <vector>
#include <tuple>

class BellmanFord : public TraversalAlgorithm {
private:
    const std::vector<std::vector<std::pair<int, int>>>& graph;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<long long> distances;
    bool finished;
    std::vector<Step> history;

    int currentStepIndex = -1;
    int start;
    int vertices;
    int currentPhase;
    int currentEdgeIndex;

public:
    BellmanFord(const std::vector<std::vector<std::pair<int, int>>>& adj, int startNode);

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
