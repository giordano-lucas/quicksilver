#include "PathStatistic.h"

void PathStatistic::construct(const std::shared_ptr<SimpleGraph> &g) {
    for(auto edges : g->adj){
        if(edges.size() != 0) distinct_s++;
    }
    for(auto edges : g->reverse_adj){
        if(edges.size() != 0) distinct_t++;
    }
    distinct_l = g->getNoLabels();
    distinct_e = g->getNoDistinctEdges();
}
PathStatistic estimateConcat(PathStatistic p);
PathStatistic estimateUnion(PathStatistic p);
PathStatistic PathStatistic::estimateGreater(uint32_t l);
PathStatistic PathStatistic::estimateLower(uint32_t l);
PathStatistic PathStatistic::estimateKleene(uint32_t l);
cardStat PathStatistic::toCardStat();
