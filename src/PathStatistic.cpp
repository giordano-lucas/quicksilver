#include "PathStatistic.h"

void PathStatistic::construct(const std::shared_ptr<SimpleGraph> &g) {
     syn1.resize(g->getNoLabels());
    for(u_int32_t s =0;  s <g->adj.size() ; ++s ){
        
        for (auto edge : edgeList){
            uint32_t l = edge.second;
            syn1[l].in++;
        }
    }
    for(auto edgeList : g->reverse_adj){
        for (auto edge : edgeList){
            syn1[edge.second].out++;
        }
    }
}
PathStatistic estimateConcat(PathStatistic p);
PathStatistic estimateUnion(PathStatistic p);
PathStatistic PathStatistic::estimateGreater(uint32_t l);
PathStatistic PathStatistic::estimateLower(uint32_t l);
PathStatistic PathStatistic::estimateKleene(uint32_t l);
cardStat PathStatistic::toCardStat();
