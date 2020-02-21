#include <cmath>
#include "SimpleGraph.h"
#include "SimpleEstimator.h"



SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){
    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    // adj holds pairs of the form (edgeLabel, to)
    // reverse_adj holds pairs of the form (edgeLabel, from)
    //printGraphInfo();

    // do your prep here
    numPairs = graph->getNoDistinctEdges();
    numLabels = graph->getNoLabels();

    // calculate the number of distinct 's' nodes
    for  (auto halfEdges : graph->adj) {
        if (halfEdges.size() > 0) {
            distinctFromNodes++;
        }
    }

    // calculate the number of distinct 't' nodes
    for  (auto halfEdges : graph->reverse_adj) {
        if (halfEdges.size() > 0) {
            distinctToNodes++;
        }
    }

}

cardStat SimpleEstimator::estimate(PathQuery *q) {

    // perform your estimation here
    PathTree *path = q->path;
    std::string s = q->s;
    std::string t = q->t;
    cardStat cardinalityStat = estimatePathTree(path);

    // process the query depending on its 's' and 't' nodes:
    if (s.compare("*")==0 and t.compare("*")==0) {
        // s is '*', t is '*'
        return cardinalityStat;
    } else if (s.compare("*")==0) {
        // s is '*', t is constant
       return cardStat{cardinalityStat.noOut/cardinalityStat.noIn, cardinalityStat.noPaths/cardinalityStat.noIn, 1};
        
    } else {
        // t is '*', s is constant
       return cardStat{1, cardinalityStat.noPaths/cardinalityStat.noOut, cardinalityStat.noIn/cardinalityStat.noOut};
    }
}

cardStat SimpleEstimator::estimatePathTree(PathTree *path) {

    // path can be only in two states: either isLeaf() or isConcat()
    if (path->isLeaf()) {
        return estimateLeaf(path->data);
    } else if (path->isConcat()) {
        cardStat leftStat = estimatePathTree(path->left);
        cardStat rightStat = estimatePathTree(path->right);
        return estimateConcat(leftStat, rightStat);
    }
}

// reg_exp is simple regular path expression
cardStat SimpleEstimator::estimateLeaf(std::string regExp) {
    if (regExp.size() != 2) {
        throw "Illegal argument";
    }

    char operation = regExp[1];
    switch(operation) {
        case '>':
            return cardStat{distinctFromNodes / numLabels,
                            numPairs / numLabels,
                            distinctToNodes / numLabels};
        case '<':
            return cardStat{distinctToNodes / numLabels,
                            numPairs / numLabels,
                            distinctFromNodes/ numLabels};
        case '+':
            // TODO: improve, it should return more than just 'l>'
            return cardStat{distinctFromNodes / numLabels,
                            numPairs / numLabels,
                            distinctToNodes / numLabels};
    }
}

cardStat SimpleEstimator::estimateConcat(cardStat left, cardStat right) {

    // join operation (slides p.49):
     uint32_t noPaths = floor(std::min(left.noPaths*(right.noPaths/right.noOut),
                                      right.noPaths*(left.noPaths/left.noIn)));

     // Nikolay comment: use only this (be aware, this brings you down from 6k to 44k on the leaderboard!)
    // uint32_t noPaths = floor(left.noPaths*(right.noPaths/right.noOut));

    // apply reduction factor:
    uint32_t noOut = left.noOut * (((double) noPaths)/left.noPaths);
    uint32_t noIn = right.noIn * (((double) noPaths)/right.noPaths);

    return cardStat {noOut, noPaths, noIn};
}

void SimpleEstimator::printGraphInfo() {
    // print all the available information about the graph:
    std::cout << "GRAPH INFO" << std::endl;
    std::cout << "Number of vertices: " << graph->getNoVertices() << std::endl;
    std::cout << "Number of edges: " <<  graph->getNoEdges() << std::endl;
    std::cout << "Number of distinct edges: " << graph->getNoDistinctEdges() << std::endl;
    std::cout << "Number of labels: " << graph->getNoLabels() << std::endl;
}
