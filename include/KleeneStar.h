//
// Created by Lucas Giordano on 24/03/2020.
//

#ifndef QUICKSILVER_KLEENESTAR_H
#define QUICKSILVER_KLEENESTAR_H

#include <EdgeIndex.h>
#include "PhysicalOperator.h"

class KleeneStar : public PhysicalOperator{
private:
    EdgeIndex* index;
    QueryEdge queryEdge;
    void dfs(std::unordered_multimap<Node,Node>& base, std::unordered_map<Node,bool>& discovered,Node n, Node source, bool reversed);
public:
    KleeneStar(EdgeIndex* index, QueryEdge queryEdge, ResultSorted resultSorted);
    uint32_t cost() const override;
    cardPathStat getCardinality() const override;
    void evalPipeline() override;
    Edge produceNextEdge() override;
    std::ostream &name(std::ostream &strm) const override;
};


#endif //QUICKSILVER_KLEENESTAR_H
