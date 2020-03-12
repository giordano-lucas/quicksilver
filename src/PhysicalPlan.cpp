//
// Created by Lucas Giordano on 12/03/2020.
//

#include "PhysicalPlan.h"

PhysicalPlan::PhysicalPlan(PhysicalOperator *op, PhysicalPlan *leftPlan, PhysicalPlan *righPlan) : op(op),
                                                                                                   leftPlan(leftPlan),
                                                                                                   righPlan(righPlan) {}

PhysicalPlan::~PhysicalPlan() {
    delete(leftPlan);
    delete(rightPlan);
}
bool PhysicalPlan::isLeaf() {
    return leftPlan == nullptr && rightPlan == nullptr;
}

void PhysicalPlan::evaluate() {
    std::thread left([this]  {leftPlan->evaluate(); });
    std::thread right([this] {rightPlan->evaluate();});


}

cardStat PhysicalPlan::getOutput() {
    return cardStat();
}
