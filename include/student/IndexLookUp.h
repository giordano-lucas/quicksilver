// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_INDEXLOOKUP_H
#define QUICKSILVER_INDEXLOOKUP_H

#include "PhysicalOperator.h"
#include "SimpleGraph.h"

class IndexLookUp : public PhysicalOperator{
private:
    std::shared_ptr<SimpleGraph>& index;
    QueryEdge queryEdge;
    bool reversed;
    std::vector<Edge> sortedResSource;
    std::vector<Edge> sortedResTarget;
    IndexResult res;
    bool resValid = true;
    bool ready = false;
public:
    IndexLookUp(std::shared_ptr<SimpleGraph>& index, QueryEdge queryEdge, bool reversed);

    ~IndexLookUp();
    void evalPipeline(ResultSorted resultSorted)  override;
    uint32_t cost() const override;
    bool isLeftBounded() const override;
    bool isRightBounded() const override;

    std::ostream &name(std::ostream &strm) const override;

protected:
    Edge produceNextEdge()  override;
};

#endif //QUICKSILVER_INDEXLOOKUP_H
