//
// Created by Lucas Giordano on 09/04/2020.
//

#ifndef QUICKSILVER_INDEXJOIN_H
#define QUICKSILVER_INDEXJOIN_H


#include "PhysicalOperator.h"

class IndexJoin : public PhysicalOperator{

private:
    Node V = 0;
public:
    IndexJoin(Node V,PhysicalOperator *left, PhysicalOperator *right);
    IndexJoin(Node V,PhysicalOperator *left, PhysicalOperator *right, ResultSorted res);
    cardStat eval() override;

    uint32_t cost() override;

    ~IndexJoin() override;

    void evalPipeline(ResultSorted resultSorted) override;
    void evalPipelineLeft();
    void evalPipelineRight();
    Edge produceNextEdge() override;

    void skip(Node until) override;

    std::ostream &name(std::ostream &strm) const override;

};


#endif //QUICKSILVER_INDEXJOIN_H
