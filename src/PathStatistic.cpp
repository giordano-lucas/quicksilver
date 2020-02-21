#include "PathStatistic.h"
#include "SimpleEstimator.h"

/**
 * Sort labels first then t node
 * */
bool PathStatistic::sortPairsL(const std::pair<uint32_t,uint32_t> &a, const std::pair<uint32_t,uint32_t> &b) {
    //fprintf(stderr, "++++ (%d,%d) vs (%d,%d) ++++ \n", a.first,a.second, b.first, b.second);
    if (a.first < b.first) return true;
    if (a.first == b.first) return a.second < b.second;
    return false;
}
void PathStatistic::construct(std::shared_ptr<SimpleGraph> &g) {
    // *** INITIALIZE SYN ***
    syn1.resize(g->getNoLabels());
    //resize syn2 to a L x L matrix
    syn2.resize(g->getNoLabels());
    for (uint32_t l1= 0  ; l1 < g->getNoLabels() ; l1++){
        syn2[l1].resize(g->getNoLabels());
    }

    // *** SYNAPSE 1 Statistics ***

    for (auto sourceVec : g->adj) {
        std::sort(sourceVec.begin(), sourceVec.end(), PathStatistic::sortPairsL);
        uint32_t prevTarget = 0;
        uint32_t prevLabel = 0;
        bool first = true;
        for (const auto &labelTgtPair : sourceVec) {
            if (first || !(prevTarget == labelTgtPair.second && prevLabel == labelTgtPair.first)) {
                first = false;
                prevTarget = labelTgtPair.second;
                prevLabel = labelTgtPair.first;
                syn1[prevLabel].path++;
            }
        }
    }

    //Loop to compute syn1.out and syn1.path
    for(auto edgeList : g->adj){
        std::vector<bool> changed(g->getNoLabels(), false);
        for (auto edge : edgeList){

            uint32_t l = edge.first;
            if (!changed[l]) { //update out only for one single edge with label l
                syn1[l].out++;
                changed[l] = true;
            }
        }
    }

    //Loop to compute syn1.in
    for(auto edgeList : g->reverse_adj){
        std::vector<bool> changed(g->getNoLabels(), false);
        for (auto edge : edgeList){
            uint32_t l = edge.first;
            if (!changed[l]) { //update in only for single one edge with label l
                syn1[l].in++;
                changed[l] = true;
            }
        }
    }
    // *** SYNAPSE 2 Statistics ***
    //compute middle and two
    for(uint32_t middle = 0; middle < g->reverse_adj.size() ; ++middle){
        std::vector<std::vector<bool>> visitL1L2(g->getNoLabels(), std::vector<bool>(g->getNoLabels(),false));
        for (auto edgeL1 : g->reverse_adj[middle]){ //use reverse adj since we are interested in incoming edge l1 to middle
            uint32_t l1 = edgeL1.first;
            if (middle < g->adj.size()) { //check that middle has at least one outgoing edge
                std::vector<bool> visitL2(g->getNoLabels(), false);
                for (auto edgeL2 : g->adj[middle]) { // - join on middle element
                    // - use adj since we are interested in outgoing edge l2 from middle
                    uint32_t l2 = edgeL2.first;
                    if (!visitL1L2[l1][l2]) {
                        visitL1L2[l1][l2] = true;
                        syn2[l1][l2].middle++;
                    }
                    if (visitL1L2[l1][l2] && !visitL2[l2]) {//compute syn2.two
                        visitL2[l2] = true;
                        syn2[l1][l2].two++;
                    }
                }
            }
        }
    }
    //compute syn2.in
    for(auto edgeListL2 : g->reverse_adj){  //use reverse adj since we are interested in incoming edge l2 to target node
        std::vector<std::vector<bool>> visitL1L2(g->getNoLabels(), std::vector<bool>(g->getNoLabels(),false));
        for (auto edgeL2 : edgeListL2){
            uint32_t l2 = edgeL2.first;
            uint32_t middle = edgeL2.second;  //interested in path l1/l2 to t
            if (middle < g->adj.size()){
                for(auto edgeL1 : g->reverse_adj[middle]) {  //use reverse adj since we are interested in incoming edge l1 to middle node
                    uint32_t l1 = edgeL1.first;
                    if (!visitL1L2[l1][l2]) {
                        visitL1L2[l1][l2] = true;
                        syn2[l1][l2].in++;
                    }
                }
            }

        }
    }
    /*for (uint32_t l1=0; l1 < syn2.size() ; ++l1){
        fprintf(stderr," Syn1[%d] : out = %d | in = %d | path = %d \n", l1, syn1[l1].out, syn1[l1].in, syn1[l1].path);
    }
    fprintf(stderr,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \n");
    for (uint32_t l1=0; l1 < syn2.size() ; ++l1){
        for (uint32_t l2=0; l2 < syn2[l1].size() ; ++l2){
            fprintf(stderr," Syn[%d][%d] : mid = %d | in = %d | two = %d \n", l1,l2, syn2[l1][l2].middle, syn2[l1][l2].in, syn2[l1][l2].two);
            if (syn2[l1][l2].middle == 0) {syn2[l1][l2].middle++;}
            if (syn2[l1][l2].in     == 0) {syn2[l1][l2].in++;}
            if (syn2[l1][l2].two    == 0) {syn2[l1][l2].two++;}
        }
    }*/
}
/* /!\ ONLY VALID FOR LEFT-DEEP PATH TREE /!\*/
cardPathStat PathStatistic::estimateConcat(cardPathStat left, cardPathStat right){
    uint32_t l1 = left.l;
    uint32_t l2 = right.l;
    cardStat res =  cardStat{
            (left.stat.noOut   * syn2[l1][l2].middle) / syn1[l1].in,
            (left.stat.noPaths * syn2[l1][l2].two)    / syn1[l1].in,
            (left.stat.noIn    * syn2[l1][l2].in)     / syn1[l1].in
                    };
    return cardPathStat{l2, res};
}

cardPathStat PathStatistic::estimateGreater(uint32_t l) {
    cardStat res =  cardStat{syn1[l].out, syn1[l].path, syn1[l].in};
    return cardPathStat{l, res};
}
cardPathStat PathStatistic::estimateLower(uint32_t l){
    cardStat res = cardStat{syn1[l].in, syn1[l].path, syn1[l].out};
    return cardPathStat{l, res};
}
cardPathStat PathStatistic::estimateUnion(cardPathStat min, cardPathStat max){
    cardStat res = cardStat{max.stat.noOut + min.stat.noOut/2, max.stat.noPaths + min.stat.noPaths/2, max.stat.noIn + min.stat.noIn/2};
    return cardPathStat{min.l, res};
}
cardPathStat PathStatistic::estimateKleene(uint32_t l){
    cardPathStat path1 = PathStatistic::estimateGreater(l); //path of length 1
    cardPathStat res   = path1;
    for (uint32_t i = 0 ; i < 2 ; ++i){
        res = PathStatistic::estimateUnion(res, PathStatistic::estimateConcat(res, path1));
    }
    return res;
}
