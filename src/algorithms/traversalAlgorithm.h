#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <utility>

class TraversalAlgorithm {
public:

    virtual ~TraversalAlgorithm() = default;

    virtual std::pair<int, int> stepForward() = 0;
    virtual std::pair<int, int> stepBackward() = 0;
    virtual bool isFinished() const = 0;
    virtual int getCurrentStepIndex() const = 0;
    virtual int getTotalSteps() const = 0;
    virtual std::pair<int, int> getHistory(int index) const = 0;
    virtual int getStartNode() const = 0;
};

#endif
