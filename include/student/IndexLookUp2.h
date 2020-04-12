//
// Created by Lucas Giordano on 09/04/2020.
//

#ifndef QUICKSILVER_INDEXLOOKUP2_H
#define QUICKSILVER_INDEXLOOKUP2_H


#include "PhysicalOperator.h"
#include "SimpleGraph.h"
class IndexLookUp2 : public PhysicalOperator{
private:
    std::shared_ptr<SimpleGraph>& index;
    Label l1;
    Label l2;
    IndexIterator res;
    bool ready = false;

public:
    IndexLookUp2(std::shared_ptr<SimpleGraph>& index, Label l1, Label l2);

    uint32_t cost() override;

    ~IndexLookUp2() override;

    cardStat eval() override;

    void evalPipeline(ResultSorted resultSorted) override;

    Edge produceNextEdge() override;

    void skip(Node until) override;

    IteratorReachable reachable(Node s) override;

    std::ostream &name(std::ostream &strm) const override;

};


#endif //QUICKSILVER_INDEXLOOKUP2_H
