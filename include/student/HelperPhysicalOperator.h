//
// Created by Lucas Giordano on 27/03/2020.
//

#ifndef QUICKSILVER_HELPERPHYSICALOPERATOR_H
#define QUICKSILVER_HELPERPHYSICALOPERATOR_H

#include "PhysicalOperator.h"
#include "SimpleGraph.h"

PhysicalOperator* ofPathTree(PathTree* tree, std::shared_ptr<SimpleGraph>& index, Node leftBounded, Node rightBounded);
PhysicalOperator* ofPathQuery(PathQuery* pq, std::shared_ptr<SimpleGraph>& index);
PhysicalOperator* ofPathTreeLargeCard(PathTree* tree, std::shared_ptr<SimpleGraph>& index, Node leftBounded, Node rightBounded);
PhysicalOperator* ofPathQueryLargeCard(PathQuery* pq, std::shared_ptr<SimpleGraph>& index);
#endif //QUICKSILVER_HELPERPHYSICALOPERATOR_H
