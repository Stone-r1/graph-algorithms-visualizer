#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <utility>


struct Step {
    int from;
    int to;
    long long distance;
};

struct BellmanStep : Step {
    bool relaxed;
    BellmanStep(int from, int to, long long distance, bool relaxed) :
        Step{from, to, distance}, relaxed(relaxed) {}

    Step toStep() const {
        return Step{from, to, distance};
    }
};

class TraversalAlgorithm {
public:

    virtual ~TraversalAlgorithm() = default;

    virtual Step stepForward() = 0;
    virtual Step stepBackward() = 0;
    virtual bool isFinished() const = 0;
    virtual int getCurrentStepIndex() const = 0;
    virtual int getTotalSteps() const = 0;
    virtual Step getHistory(int index) const = 0;
    virtual int getStartNode() const = 0;
};

#endif
