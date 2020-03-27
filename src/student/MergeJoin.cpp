//
// Created by Lucas Giordano on 17/03/2020.
//

#include "MergeJoin.h"

Edge MergeJoin::produceNextEdge() {
    if (!ready) out.pop();
    if (!res.empty()) {
        Edge r = res.back();
        res.pop_back();
        while (lastEdgeProduced == r && !res.empty()){
            r = res.back();
            res.pop_back();
        }
        bool isEnd = res.empty() && lastEdgeProduced == r;
        lastEdgeProduced = r;
        return (isEnd)?END_EDGE:r;
    }
    return END_EDGE;
}

void MergeJoin::evalPipeline(ResultSorted resultSorted) {
    std::thread thdLeft([this] {
        left->evalPipeline(TARGET_SORTED);
    });
    std::thread thdRight([this] {
        right->evalPipeline(SOURCE_SORTED);
    });
    Edge r = right->produceNextEdge();
    Edge l = left->produceNextEdge();
    std::vector<Edge> setL;
    while(!(r==END_EDGE) && !(l == END_EDGE)){
        while (l != END_EDGE && l.target < r.source) { //advance until we have a joining edge
            l = left->produceNextEdge();
        }
        setL.clear();
        Edge tl = l;
        bool done = false;
        while (!done && !(l == END_EDGE)){ //setL contains all edges in Left that have the same target value
            if (tl.target == l.target){
                setL.push_back(l);
                l = left->produceNextEdge();
            }
            else done = true;
        }
        while (!(r == END_EDGE) && r.source < tl.target) { //advance until we have a joining edge
            r = right->produceNextEdge();
        }
        while (!(r == END_EDGE) && tl.target == r.source){
            for(auto tl : setL){
                res.push_back(Edge{tl.source,r.target});
            }
            r = right->produceNextEdge();
        }
    }
    left->terminate();
    right->terminate();
    thdLeft.join();
    thdRight.join();
    std::sort(res.begin(),res.end(), targetCompDesc);
    ready = true;
    out.push(END_EDGE,true);
}

uint32_t MergeJoin::cost() const {
    return left->getCardinality().noPaths + right->getCardinality().noPaths + left->cost() + right->cost();
}

MergeJoin::MergeJoin(PhysicalOperator *left, PhysicalOperator *right) : PhysicalOperator(
        left, right,TARGET_SORTED), res() {}

std::ostream &MergeJoin::name(std::ostream &strm) const {
    return strm << "MergeJoin";
}


