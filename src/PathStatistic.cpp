#include "PathStatistic.h"
#include "SimpleEstimator.h"

void PathStatistic::construct(const std::shared_ptr<SimpleGraph> &g) {
    // *** SYNAPSE 1 Statistics ***
    syn1.resize(g->getNoLabels());
    for(auto edgeList : g->adj){
        std::vector<bool> changed(g->getNoLabels(), false);
        for (auto edge : edgeList){
            uint32_t l = edge.second;
            if (!changed[l]) { //update out only for one sinqle edge with label l
                syn1[l].out++;
                changed[l] =true;
            }
            syn1.path++;      //update path
        }
    }
    for(auto edgeList : g->reverse_adj){
        std::vector<bool> changed(g->getNoLabels(), false);
        for (auto edge : edgeList){
            uint32_t l = edge.second;
            if (!changed[l]) { //update in only for single one edge with label l
                syn1[l].in++;
                changed[l] = true;
            }
        }
    }
    // *** SYNAPSE 2 Statistics ***

    /* TO DO */
}
/* /!\ ONLY VALID FOR LEFT-DEEP PATH TREE /!\*/
cardStat estimateConcat(cardStat c){
    return cardStat{c.noOut * (syn2[l1][l2].middle / syn1[l1].in),c.noPaths * (syn2[l1][l2].two / syn1[l1].in),c.in * (syn2[l1][l2].in / syn1[l1].in)};
}

cardStat PathStatistic::estimateGreater(uint32_t l) {
    return cardStat{syn1[l].out, syn1[l].pairs, syn1[l].in};
}
cardStat PathStatistic::estimateLower(uint32_t l){
    return cardStat{syn1[l].in, syn1[l].pairs, syn1[l].out};
}
cardStat estimateUnion(cardStat min, cardStat max){
    return cardStat{max.noOut + min.noOut/2, max.path + min.path/2, max.noIn + min.noIn/2};
}
cardStat PathStatistic::estimateKleene(uint32_t l){
    cardStat res = estimateGreater(l); //path of length 1     
    for (uint32_t i = 0 ; i < 4 ; ++i){
        res = estimateUnion(res, estimateConcat(res))
    }
    return res;
}
