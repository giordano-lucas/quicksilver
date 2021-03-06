#include <PhysicalOperator.h>
#include <IndexLookUp.h>
#include <KleeneStar.h>
#include <MergeJoin.h>
#include <student/HelperPhysicalOperator.h>
#include <student/Planner.h>
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
   // PhysicalOperator* gen_plan = ofPathQueryLargeCard(query, graph);
    //op->attachEstimator(est);
   // std::cout << *op << std::endl;
   // auto res = op->eval();
   // delete op;
   // return res;

    Planner* query_planner = new Planner();
    //auto start = std::chrono::steady_clock::now();
    PhysicalOperator* gen_plan = query_planner->generatePlan(query, graph, est);
    gen_plan = reduce(gen_plan, nullptr,graph);
    // auto end = std::chrono::steady_clock::now();
    //long localEvalTime = std::chrono::duration<double, std::milli>(end - start).count();
    //std::cout << "Time to generate plan (logical optimization): " << localEvalTime << " ms" << std::endl;
    //std::cout << "COST OF THE PHYS OPERATOR: " << gen_plan->cost() << std::endl;
    auto res = gen_plan->eval();
    //std::cout << *gen_plan << std::endl;
    delete gen_plan;
    //res.noPaths++;
    return res;
}
