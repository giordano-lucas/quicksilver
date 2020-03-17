//
// Created by Lucas Giordano on 17/03/2020.
//

#include "MergeJoin.h"

OutEdge MergeJoin::produceNextEdge() {
    return out.pop();
}

void MergeJoin::evalPipeline() {
    std::thread thdLeft([this] {
        left->evalPipeline();
    });
    std::thread thdRight([this] {
        right->evalPipeline();
    });

    OutEdge r;
    OutEdge l;
    std::vector<OutEdge> setL;
    while(!((r = right->produceNextEdge())==END_EDGE) && !((l = left->produceNextEdge()) == END_EDGE)){
        setL.clear();
        OutEdge tl = l;
        bool done = false;
        while (!done && !(l == END_EDGE)){ //setL contains all edges in Left that have the same target value
            if (tl.target == l.target){
                setL.push_back(l);
                l = left->produceNextEdge();
            }
            else done = true;
        }
        while (!(r == END_EDGE) && r.source < l.target) //advance until we have a joining edge
            r = right->produceNextEdge();
        while(!(r == END_EDGE)){
            for(auto tl : setL){
                out.push(OutEdge{l.source,r.target},false);
            }
            r = right->produceNextEdge();
        }
    }
    out.push(END_EDGE,true);

    thdRight.join();
    thdLeft.join();
}

uint32_t MergeJoin::cost() const {
    return 0;
}

MergeJoin::MergeJoin(PhysicalOperator *left, PhysicalOperator *right) : PhysicalOperator(
        left, right, TARGET_SORTED) {}

std::ostream &MergeJoin::name(std::ostream &strm) const {
    return strm << "MergeJoin";
}


