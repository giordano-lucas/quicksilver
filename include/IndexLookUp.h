//
// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_INDEXLOOKUP_H
#define QUICKSILVER_INDEXLOOKUP_H

#include "PhysicalOperator.h"
#include "EdgeIndex.h"
class IndexLookUp : PhysicalOperator{
private:
    EdgeIndex* index;
    Edge prefixEdge;
    bool reversed;
public:
    IndexLookUp(Queue *out, std::mutex *mutexOut, EdgeIndex* index, Edge prefixEdge, bool reversed);

    ~IndexLookUp();

    cardStat eval() const override;
    void evalPipeline() const override;
    uint32_t cost() const override;
};


#endif //QUICKSILVER_INDEXLOOKUP_H
