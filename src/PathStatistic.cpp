#include "PathStatistic.h"
#include "SimpleEstimator.h"

void PathStatistic::construct(const std::shared_ptr<SimpleGraph> &g) {
    // *** SYNAPSE 1 Statistics ***
    syn1.resize(g->getNoLabels());

    //Loop to compute syn1.out and syn1.path
    for(auto edgeList : g->adj){
        std::vector<bool> changed(g->getNoLabels(), false);
        for (auto edge : edgeList){
            uint32_t l = edge.second;
            if (!changed[l]) { //update out only for one sinqle edge with label l
                syn1[l].out++;
                changed[l] =true;
            }
            syn1[l].path++;      //update path
        }
    }
    //Loop to compute syn1.in
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
    //resize syn2
    syn2.resize(g->getNoLabels()); //each label has at least one associated pair (at least the pair (l,l) )
    for (uint32_t l1= 0  ; l1 < g->getNoLabels() ; l1++){  // for example : label 0 can be associated with labels in {0,1,...,L-1} => size of L
                                                           //               label 1 can not be associated with label 0             => size of L-1
                                                           //               label l1 can not be associated with label {0,..,l1-1}  => size of L-l1
        syn2[l1].resize(g->getNoLabels() - l1);
    }
    //compute middle
    for(uint32_t middle = 0; middle < g->reverse_adj.size() ; ++middle){
        std::vector<bool> visitL1(g->getNoLabels(), false);
        for (auto edgel1 : g->reverse_adj[middle]){ //use reverse adj since we are interested in incoming edge l1 to middle
            uint32_t l1 = edgel1.second;
            if (!visitL1[l1] && middle < g->adj.size()) { //check that middle has at least one outgoing edge
                visitL1[l1] = true;
                std::vector<bool> visitL2(g->getNoLabels(), false);
                for (auto edgel2 : g->adj[middle]){ // - join on middle element
                                                    // - use adj since we are interested in outgoing edge l2 from middle
                    uint32_t l2 = edgel2.second;
                    if (!visitL2[l2]) {
                        visitL2[l2] = true;
                        if  (l1 <= l2)  {syn2[l1][l2].middle++;} //check that syn2 can be indexed with [l1][l2]
                        else            {syn2[l2][l1].middle++;}
                    }
                }
            }
        }
    }
    //compute syn2.in
    for(auto edgeListL2 : g->reverse_adj){  //use reverse adj since we are interested in incoming edge l2 to target node
        std::vector<std::vector<bool>> visitL1L2(g->getNoLabels(), std::vector<bool>(g->getNoLabels(),false));
        for (auto edgeL2 : edgeListL2){
            uint32_t l2 = edgeL2.second;
            uint32_t middle = edgeL2.first;  //interested in path l1/l2 to t
            if (middle < g->adj.size()){
                for(auto edgeL1 : g->reverse_adj[middle]) {  //use reverse adj since we are interested in incoming edge l1 to middle node
                    uint32_t l1 = edgeL1.second;
                    if (!visitL1L2[l2][l1]) {
                        visitL1L2[l2][l1] = true;
                        if  (l1 <= l2)  {syn2[l1][l2].in++;} //check that syn2 can be indexed with [l1][l2]
                        else            {syn2[l2][l1].in++;}
                    }
                }
            }

        }
    }
    /* TO DO */
}
/* /!\ ONLY VALID FOR LEFT-DEEP PATH TREE /!\*/
cardPathStat PathStatistic::estimateConcat(cardPathStat left, cardPathStat right){
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
cardPathStat PathStatistic::estimateUnion(cardPathStat min, cardPathStat max){
    cardStat res = cardStat{max.stat.noOut + min.stat.noOut/2, max.stat.noPaths + min.stat.noPaths/2, max.stat.noIn + min.stat.noIn/2};
    return cardPathStat{min.l, res};
}
cardPathStat PathStatistic::estimateKleene(uint32_t l){
    cardPathStat path1 = PathStatistic::estimateGreater(l); //path of length 1
    cardPathStat res   = path1;
    for (uint32_t i = 0 ; i < 4 ; ++i){
        res = PathStatistic::estimateUnion(res, PathStatistic::estimateConcat(res, path1));
    }
    return res;
}
