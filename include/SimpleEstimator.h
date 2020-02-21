#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"
#include "PathStatistic.h"

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override ;
    cardStat estimate(PathQuery *q) override ;

private:
    PathStatistic pathStatistic;
    void printGraphInfo();
    cardStat estimateLeaf(std::string reg_exp);
    cardStat estimatePathTree(PathTree *p);
    cardStat estimateConcat(cardStat left, cardStat right);
};


#endif //QS_SIMPLEESTIMATOR_H
