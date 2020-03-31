//
// Created by Lucas Giordano on 24/03/2020.
//

#ifndef QUICKSILVER_KLEENESTAR_H
#define QUICKSILVER_KLEENESTAR_H

#include <SimpleGraph.h>
#include "PhysicalOperator.h"

class KleeneStar : public PhysicalOperator{
private:
    std::shared_ptr<SimpleGraph>& index;
    QueryEdge queryEdge;
    bool done = false;
    void dfs(std::unordered_map<Node,bool>& discovered,Node n, Node source, bool reversed,std::vector<Edge>& output);
    void cacheDfs(std::unordered_map<Node,bool>& discovered,Node n ,bool reversed,bool firstIt);
    std::unordered_map<Node,std::vector<Node>> cache;
    std::unordered_map<Node,bool> finishedCache;
    void pushEdges(std::vector<Edge> &edges, bool reversed);
public:
    KleeneStar(std::shared_ptr<SimpleGraph>& index, QueryEdge queryEdge);
    uint32_t cost() override;
    void evalPipeline(ResultSorted resultSorted) override;
    Edge produceNextEdge() override;
    std::ostream &name(std::ostream &strm) const override;
};


#endif //QUICKSILVER_KLEENESTAR_H
