//
// Created by Lucas Giordano on 17/03/2020.
//

#ifndef QUICKSILVER_MERGEJOIN_H
#define QUICKSILVER_MERGEJOIN_H


#include "PhysicalOperator.h"

class MergeJoin : public PhysicalOperator{


public:
   // cardStat eval() override;

private:
    std::vector<Edge> res;
    size_t  idx = 0;
    std::vector<Edge> extraOutput;
    Edge lastEdgeProduced = END_EDGE;
    bool ready = false;

    void removeDuplicate(std::vector<Edge>& array, Edge& last);
    size_t binarySearch(std::vector<Edge>& arr, size_t  low, Node key);
public:
    MergeJoin(PhysicalOperator *left, PhysicalOperator *right);
    void evalPipeline(ResultSorted resultSorted) override;
    Edge produceNextEdge() override;
    uint32_t cost() override;
    std::ostream &name(std::ostream &strm) const override;
    void skip(Node until) override;
};


#endif //QUICKSILVER_MERGEJOIN_H
