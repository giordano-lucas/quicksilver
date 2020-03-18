//


// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_INDEXLOOKUP_H
#define QUICKSILVER_INDEXLOOKUP_H

#include "PhysicalOperator.h"
#include "EdgeIndex.h"
class IndexLookUp : public PhysicalOperator{
private:
    EdgeIndex* index;
    Edge prefixEdge;
    bool reversed;
    std::vector<Edge> sortedResSource;
    std::vector<Edge> sortedResTarget;
    IndexResult res;
    bool resValid = true;
    bool ready = false;
public:
    IndexLookUp(EdgeIndex* index, Edge prefixEdge, bool reversed, ResultSorted resultSorted);

    ~IndexLookUp();

    void evalPipeline()  override;
    uint32_t cost() const override;
    bool isLeftBounded() const override;
    bool isRightBounded() const override;

    std::ostream &name(std::ostream &strm) const override;

protected:
    OutEdge produceNextEdge()  override;
};


#endif //QUICKSILVER_INDEXLOOKUP_H
