#include <cmath>
#include "SimpleGraph.h"
#include "SimpleEstimator.h"



SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){
    // works only with SimpleGraph
    graph = g;
}
// sort on the second item in the pair, then on the first (ascending order)

void SimpleEstimator::prepare() {

    // *** INITIALIZE SYN ***
    syn1.resize(graph->getNoLabels());
    //resize syn2 to a L x L matrix
    syn2.resize(graph->getNoLabels());
    for (uint32_t l1= 0  ; l1 < graph->getNoLabels() ; l1++){
        syn2[l1].resize(graph->getNoLabels());
    }

    // *** SYNAPSE 1 Statistics ***

    for (auto sourceVec : graph->adj) {
        std::sort(sourceVec.begin(), sourceVec.end(), SimpleEstimator::sortPairsL);
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
    for(auto edgeList : graph->adj){
        std::vector<bool> changed(graph->getNoLabels(), false);
        for (auto edge : edgeList){

            uint32_t l = edge.first;
            if (!changed[l]) { //update out only for one single edge with label l
                syn1[l].out++;
                changed[l] = true;
            }
        }
    }

    //Loop to compute syn1.in
    for(auto edgeList : graph->reverse_adj){
        std::vector<bool> changed(graph->getNoLabels(), false);
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
    for(uint32_t middle = 0; middle < graph->reverse_adj.size() ; ++middle){
        std::vector<std::vector<bool>> visitL1L2(graph->getNoLabels(), std::vector<bool>(graph->getNoLabels(),false));
        for (auto edgeL1 : graph->reverse_adj[middle]){ //use reverse adj since we are interested in incoming edge l1 to middle
            uint32_t l1 = edgeL1.first;
            if (middle < graph->adj.size()) { //check that middle has at least one outgoing edge
                for (auto edgeL2 : graph->adj[middle]) { // - join on middle element
                    // - use adj since we are interested in outgoing edge l2 from middle
                    uint32_t l2 = edgeL2.first;
                    if (!visitL1L2[l1][l2]) {
                        visitL1L2[l1][l2] = true;
                        syn2[l1][l2].middle++;
                    }
                }
            }
        }
    }
    //compute syn2.in
    for(auto edgeListL2 : graph->reverse_adj){  //use reverse adj since we are interested in incoming edge l2 to target node
        std::vector<std::vector<bool>> visitL1L2(graph->getNoLabels(), std::vector<bool>(graph->getNoLabels(),false));
        for (auto edgeL2 : edgeListL2){
            uint32_t l2 = edgeL2.first;
            uint32_t middle = edgeL2.second;  //interested in path l1/l2 to t
            bool visitL2 = false;
            if (middle < graph->adj.size()){
                for(auto edgeL1 : graph->reverse_adj[middle]) {  //use reverse adj since we are interested in incoming edge l1 to middle node
                    uint32_t l1 = edgeL1.first;
                    if (!visitL1L2[l1][l2]) {
                        visitL1L2[l1][l2] = true;
                        syn2[l1][l2].in++;
                    }
                    if (!visitL2) {
                        visitL2 = true;
                        syn2[l1][l2].two++;
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

cardStat SimpleEstimator::estimate(PathQuery *q) {

    // perform your estimation here
    PathTree *path = q->path;
    std::string s = q->s;
    std::string t = q->t;
    cardPathStat cardinalityPathStat = estimatePathTree(path); 
    cardStat cardinalityStat = cardinalityPathStat.stat;

    // process the query depending on its 's' and 't' nodes:
    if (s.compare("*")==0 and t.compare("*")==0) {
        // s is '*', t is '*'
        return cardinalityStat;
    } else if (s.compare("*")==0) {
        // s is '*', t is constant
        return cardStat{
                static_cast<uint32_t>(std::ceil(((double) (cardinalityStat.noOut))/ cardinalityStat.noIn)),
                static_cast<uint32_t>(std::ceil(((double) (cardinalityStat.noPaths)) / cardinalityStat.noIn)),
                1
        };
    } else {
        // t is '*', s is constant
        return cardStat{
                1,
                static_cast<uint32_t>(std::ceil(((double) (cardinalityStat.noPaths)) / cardinalityStat.noOut)),
                static_cast<uint32_t>(std::ceil(((double) (cardinalityStat.noIn)) / cardinalityStat.noOut))
        };
}
}

cardPathStat SimpleEstimator::estimatePathTree(PathTree *path) {

    // path can be only in two states: either isLeaf() or isConcat()
    if (path->isLeaf()) {
        return estimateLeaf(path->data);
    } else if (path->isConcat()) {
        cardPathStat leftStat = estimatePathTree(path->left);
        cardPathStat rightStat = estimatePathTree(path->right);
        return SimpleEstimator::estimateConcat(leftStat, rightStat);
    }
    throw "Illegal argument";
}

// reg_exp is simple regular path expression
cardPathStat SimpleEstimator::estimateLeaf(std::string regExp) {
    if (regExp.size() != 2) {
        throw "Illegal argument";
    }
    uint32_t l = (regExp[0] - '0');  // NEED CORRECTION : what if l is of multiple digits ?
    char operation = regExp[1];
    switch(operation) {
        case '>': return SimpleEstimator::estimateGreater(l);
        case '<': return SimpleEstimator::estimateLower(l);
        case '+': return SimpleEstimator::estimateKleene(l);
    }
    throw "Illegal argument";
}

void SimpleEstimator::printGraphInfo() {
    // print all the available information about the graph:
    std::cout << "GRAPH INFO" << std::endl;
    std::cout << "Number of vertices: " << graph->getNoVertices() << std::endl;
    std::cout << "Number of edges: " <<  graph->getNoEdges() << std::endl;
    std::cout << "Number of distinct edges: " << graph->getNoDistinctEdges() << std::endl;
    std::cout << "Number of labels: " << graph->getNoLabels() << std::endl;
}
//*********************************************************************************************

bool SimpleEstimator::sortPairsL(const std::pair<uint32_t,uint32_t> &a, const std::pair<uint32_t,uint32_t> &b) {
    if (a.first < b.first) return true;
    if (a.first == b.first) return a.second < b.second;
    return false;
}

/* /!\ ONLY VALID FOR LEFT-DEEP PATH TREE /!\*/
cardPathStat SimpleEstimator::estimateConcat(cardPathStat left, cardPathStat right){
    uint32_t l1 = left.l;
    uint32_t l2 = right.l;
    cardStat res;
    if ((left.lastOp == greater && right.lastOp == lower)){
        res =  cardStat{
                left.stat.noOut,
                static_cast<uint32_t>(left.stat.noPaths * (((double)right.stat.noPaths) / syn1[l1].in)),
                right.stat.noOut
        };
    }
    else if ((left.lastOp == lower && right.lastOp == greater)){
        res =  cardStat{
                left.stat.noIn,
                static_cast<uint32_t>(left.stat.noPaths * (((double)right.stat.noPaths) / syn1[l1].in)),
                right.stat.noIn
        };
    }
    else {
        res =  cardStat{
                static_cast<uint32_t>(left.stat.noOut * (((double)syn2[l1][l2].middle )/ syn1[l1].in)),
                static_cast<uint32_t>(left.stat.noPaths * (((double)syn2[l1][l2].two) / syn1[l1].in)),
                static_cast<uint32_t>(left.stat.noIn * (((double)syn2[l1][l2].in) / syn1[l1].in))
        };
    }

    return cardPathStat{right.lastOp,l2, res};
}

cardPathStat SimpleEstimator::estimateGreater(uint32_t l) {
    cardStat res =  cardStat{syn1[l].out, syn1[l].path, syn1[l].in};
    return cardPathStat{greater,l, res};
}
cardPathStat SimpleEstimator::estimateLower(uint32_t l){
    cardStat res = cardStat{syn1[l].in, syn1[l].path, syn1[l].out};
    return cardPathStat{lower,l, res};
}
cardPathStat SimpleEstimator::estimateUnion(cardPathStat min, cardPathStat max){
    cardStat res = cardStat{max.stat.noOut + min.stat.noOut/2, max.stat.noPaths + min.stat.noPaths/2, max.stat.noIn + min.stat.noIn/2};
    return cardPathStat{min.lastOp,min.l, res};
}
cardPathStat SimpleEstimator::estimateKleene(uint32_t l){
    cardPathStat path1 = estimateGreater(l); //path of length 1
    cardPathStat res   = path1;
    for (uint32_t i = 0 ; i < 2 ; ++i){
        res = estimateUnion(res, estimateConcat(res, path1));
    }
    return res;
}