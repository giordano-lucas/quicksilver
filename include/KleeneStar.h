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
    bool done = false;
    void dfs(std::unordered_map<Node,bool>& discovered,Node n, Node source, bool reversed,std::vector<Edge>& output);
public:
    KleeneStar(EdgeIndex* index, QueryEdge queryEdge);
    uint32_t cost() const override;
    cardPathStat getCardinality() const override;
    void evalPipeline(ResultSorted resultSorted) override;
    Edge produceNextEdge() override;
    std::ostream &name(std::ostream &strm) const override;
};


#endif //QUICKSILVER_KLEENESTAR_H
