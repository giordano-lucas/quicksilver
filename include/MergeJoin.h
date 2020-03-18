//
// Created by Lucas Giordano on 17/03/2020.
//

#ifndef QUICKSILVER_MERGEJOIN_H
#define QUICKSILVER_MERGEJOIN_H


#include "PhysicalOperator.h"

class MergeJoin : public PhysicalOperator{



private:
    std::vector<OutEdge> res;
    bool ready = false;

public:
    MergeJoin(PhysicalOperator *left, PhysicalOperator *right);
    void evalPipeline() override;
    OutEdge produceNextEdge() override;
    uint32_t cost() const override;
    std::ostream &name(std::ostream &strm) const override;
};


#endif //QUICKSILVER_MERGEJOIN_H
