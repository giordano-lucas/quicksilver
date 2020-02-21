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
cardPathStat estimateConcat(cardPathStat left, cardPathStat right){
    uint32_t l1 = left.l;
    uint32_t l2 = right.l;
    Syn2 syn2concat = (l1 <= l2)? syn2[l1][l2]: syn2[l2][l1];
    cardStat res =  cardStat{
                        left.stat.noOut   * (syn2concat.middle / syn1[l1].in),
                        left.stat.noPaths * (syn2concat.two    / syn1[l1].in),
                        left.stat.noIn    * (syn2concat.in     / syn1[l1].in)
                    };
    return cardPathStat{l2, res};
}

cardPathStat PathStatistic::estimateGreater(uint32_t l) {
    cardStat res =  cardStat{syn1[l].out, syn1[l].pairs, syn1[l].in};
    return cardPathStat{l, res};
}
cardPathStat PathStatistic::estimateLower(uint32_t l){
    cardStat res = cardStat{syn1[l].in, syn1[l].pairs, syn1[l].out};
    return cardPathStat{l, res};
}
cardStat estimateUnion(cardStat min, cardStat max){
    return cardStat{max.noOut + min.noOut/2, max.path + min.path/2, max.noIn + min.noIn/2};
}
cardPathStat PathStatistic::estimateKleene(uint32_t l){
    cardStat res = estimateGreater(l); //path of length 1     
    for (uint32_t i = 0 ; i < 4 ; ++i){
        res = estimateUnion(res, estimateConcat(res))
    }
    return cardPathStat{l,res};
}
