//
// Created by Lucas Giordano on 09/04/2020.
//

#include "IndexLookUp2.h"

uint32_t IndexLookUp2::cost() {
    return 0;
}

IndexLookUp2::~IndexLookUp2() {

}

cardStat IndexLookUp2::eval() {
    cardStat stats {};
    std::vector<std::vector<Node>>& adj = index->adjLabel2[l1][l2];
    //std::vector<std::vector<Node>>& revadj = index->revAdjLabel2[l1][l2];
    std::vector<bool> hashIn(index->getNoVertices(), false);
    for (auto sourceVec : adj) {
        std::sort(sourceVec.begin(), sourceVec.end());
        uint32_t prevTarget = 0;
        bool first = true;
        for (const auto &labelTgt: sourceVec) {
            if (first || !(prevTarget == labelTgt)) {
                first = false;
                stats.noPaths++;
                prevTarget = labelTgt;
                if (hashIn[labelTgt] == false) {
                    hashIn[labelTgt] = true;
                    stats.noIn++;
                }
            }
        }
    }
    for(int source = 0; source < index->getNoVertices(); source++) {
        if(!adj[source].empty()) stats.noOut++;
    }
//    for(int target = 0; target < index->getNoVertices(); target++) {
 //       if(!revadj[target].empty()) stats.noIn++;
 //   }
    return stats;
}

void IndexLookUp2::evalPipeline(ResultSorted resultSorted) {
    // throw "uniplemented";
}

Edge IndexLookUp2::produceNextEdge() {
    if (source >= index->getNoVertices()) return END_EDGE;
    while(source >= index->getNoVertices()&&adj[source].empty()) source++;
    if (source >= index->getNoVertices()) return END_EDGE;
    Edge r = Edge{source,adj[source][idxTarget]};
    idxTarget++;
    if (idxTarget >= adj[source].size()) {source++; idxTarget = 0;}
    return r;
}

void IndexLookUp2::skip(Node until) {
    while(source >= index->getNoVertices()&&adj[source].empty()) source++;
}

std::vector<Node> &IndexLookUp2::reachable(Node s) {
    return adj[s];
}

std::ostream &IndexLookUp2::name(std::ostream &strm) const {
    return strm << "IndexLookUp2 (" << l1 <<  "," << l2 << ")";
}

IndexLookUp2::IndexLookUp2(std::shared_ptr<SimpleGraph>& index, Label l1, Label l2)
        : PhysicalOperator(nullptr, nullptr, NOT_SORTED), index(index),l1(l1),l2(l2), adj(index->adjLabel2[l1][l2]) {

    query.push_back(basic_query_t{greater, l1});
    query.push_back(basic_query_t{greater, l2});
}
