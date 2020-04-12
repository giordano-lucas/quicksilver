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
    return cardStat{index->sourceIndex2[l1][l2].nbHeaders, index->sourceIndex2[l1][l2].nbEdges, index->syn2[l1][l2].in};;
}

void IndexLookUp2::evalPipeline(ResultSorted resultSorted) {
    res = index->getEdgesTarget2(l1,l2);
    ready = true;
    out.push(END_EDGE, true);
}

Edge IndexLookUp2::produceNextEdge() {
    if (!ready) out.pop();
    if (res.hasNext()) {
        Edge r = *res;
        ++res;
        return r;
    }
    return END_EDGE;
}

void IndexLookUp2::skip(Node until) {
    while ( (*res).source < until) res.skip();
}

IteratorReachable IndexLookUp2::reachable(Node s) {
   return index->targetsReachable2(l1,l2,s);
}

std::ostream &IndexLookUp2::name(std::ostream &strm) const {
    return strm << "IndexLookUp2 (" << l1 <<  "," << l2 << ")";
}

IndexLookUp2::IndexLookUp2(std::shared_ptr<SimpleGraph>& index, Label l1, Label l2)
        : PhysicalOperator(nullptr, nullptr, SOURCE_SORTED, LOOKUP), index(index),l1(l1),l2(l2){

    query.push_back(basic_query_t{greater, l1});
    query.push_back(basic_query_t{greater, l2});
}
