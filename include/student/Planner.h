//
// Created by Valeriya Prokopova on 23/03/2020.
//

#ifndef QUICKSILVER_PLANNER_H
#define QUICKSILVER_PLANNER_H

#include <Subtree.h>
#include "PhysicalOperator.h"

class Planner {
public:
    ~Planner() = default;

    static PhysicalOperator *
    generatePlan(PathQuery *query, std::shared_ptr<SimpleGraph> &index, std::shared_ptr<SimpleEstimator> &e);

    static void testPlanHash(std::shared_ptr<SimpleGraph> &index);

    static void printContentsOfHash(std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash> planMap);

    static void
    deleteContentsOfHash(std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash> *planMap, Subtree fullSubtree);

    static void printCosts(std::vector<int> costs);

    static PhysicalOperator *generatePlanForUnboundedQuery(PathQuery *query, std::shared_ptr<SimpleGraph> &index,
                                                           std::shared_ptr<SimpleEstimator> &e);
};

#endif //QUICKSILVER_PLANNER_H
