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
    std::vector<Edge> extraOutput;
    Edge lastEdgeProduced = END_EDGE;
    bool ready = false;
    uint32_t  MIN_SIZE_THREAD = 500000;
    Comparator cmp= nullptr;

public:
    MergeJoin(PhysicalOperator *left, PhysicalOperator *right);
    void evalPipeline(ResultSorted resultSorted) override;
    Edge produceNextEdge() override;
    uint32_t cost() override;
    std::ostream &name(std::ostream &strm) const override;
};


#endif //QUICKSILVER_MERGEJOIN_H
