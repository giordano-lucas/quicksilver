#include <PhysicalOperator.h>
#include <IndexLookUp.h>
#include <KleeneStar.h>
#include <MergeJoin.h>
#include <student/HelperPhysicalOperator.h>
#include "SimpleEstimator.h"
#include "SimpleEvaluator.h"

SimpleEvaluator::SimpleEvaluator(std::shared_ptr<SimpleGraph> &g) {
    // works only with SimpleGraph
    graph = g;
    est = nullptr; // estimator not attached by default
}

void SimpleEvaluator::attachEstimator(std::shared_ptr<SimpleEstimator> &e) {
    est = e;
}

void SimpleEvaluator::prepare() {

    // if attached, prepare the estimator
    if(est != nullptr) est->prepare();

    // prepare other things here.., if necessary

}

cardStat SimpleEvaluator::evaluate(PathQuery *query) {
    PhysicalOperator* op = ofPathQuery(query, graph);
    op->attachEstimator(est);
    auto res = op->eval();
    delete op;
    return res;
}
