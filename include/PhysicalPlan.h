//
// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_PHYSICALPLAN_H
#define QUICKSILVER_PHYSICALPLAN_H

#include "PhysicalOperator.h"
#include "Estimator.h"
#include <thread>
class PhysicalPlan {
    PhysicalOperator* op; //use pointer to achieve dynamic link resolution
    PhysicalPlan* leftPlan;
    PhysicalPlan* rightPlan;

public:
    virtual ~PhysicalPlan();
    PhysicalPlan(PhysicalOperator *op, PhysicalPlan *leftPlan, PhysicalPlan *rightPlan);
    bool isLeaf();
    void evaluate();
    cardStat getOutput();
};


#endif //QUICKSILVER_PHYSICALPLAN_H
