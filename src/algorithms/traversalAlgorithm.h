#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <utility>


struct Step {
    int from;
    int to;
    long long distance;
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
