//
// Created by Lucas Giordano on 28/03/2020.
//

#ifndef QUICKSILVER_HASHJOIN_H
#define QUICKSILVER_HASHJOIN_H


#include "PhysicalOperator.h"

class HashJoin : public PhysicalOperator{
private:
    bool done = false;
public:
    HashJoin(PhysicalOperator *left, PhysicalOperator *right, ResultSorted defaultResultSorted):PhysicalOperator(left,right,NOT_SORTED){};
    uint32_t cost() const override;
    ~HashJoin() override;
    bool isLeftBounded() const override;
    bool isRightBounded() const override;
    void evalPipeline(ResultSorted resultSorted) override;
    Edge produceNextEdge() override;
    std::ostream &name(std::ostream &strm) const override;
};


#endif //QUICKSILVER_HASHJOIN_H
