//
// Created by Lucas Giordano on 17/03/2020.
//

#ifndef QUICKSILVER_MERGEJOIN_H
#define QUICKSILVER_MERGEJOIN_H


#include "PhysicalOperator.h"

class MergeJoin : PhysicalOperator{
protected:
    void evalPipeline() override;

private:
    std::ostream &name(std::ostream &strm) const override;

public:
    MergeJoin(PhysicalOperator *left, PhysicalOperator *right);

    OutEdge produceNextEdge() override;
    uint32_t cost() const override;

};


#endif //QUICKSILVER_MERGEJOIN_H
