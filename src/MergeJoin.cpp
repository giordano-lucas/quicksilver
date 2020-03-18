//
// Created by Lucas Giordano on 17/03/2020.
//

#include "MergeJoin.h"

OutEdge MergeJoin::produceNextEdge() {
    if (!ready) out.pop();
    if (!res.empty()) {
        OutEdge r = res.back();
        res.pop_back();
        return r;
    }
    return END_EDGE;
}

void MergeJoin::evalPipeline() {
    std::thread thdLeft([this] {
        left->evalPipeline();
    });
    thdLeft.join();
    std::thread thdRight([this] {
        right->evalPipeline();
    });
    thdRight.join();



    OutEdge r = right->produceNextEdge();
    OutEdge l = left->produceNextEdge();
    std::cout << "START LEFT => " << l << " & START RIGHT => " << r;
    std::vector<OutEdge> setL;
    while(!(r==END_EDGE) && !(l == END_EDGE)){
        setL.clear();
        OutEdge tl = l;
        bool done = false;
        while (!done && !(l == END_EDGE)){ //setL contains all edges in Left that have the same target value
            if (tl.target == l.target){
                setL.push_back(l);
                l = left->produceNextEdge();
                std::cout << "+++ Compute SET => " << l;
            }
            else done = true;
        }
        while (!(r == END_EDGE) && r.source < tl.target) { //advance until we have a joining edge
            r = right->produceNextEdge();
            std::cout << "--- REMOVE R to match with tl => " << r <<  " (tl=)"<< tl;
        }
        while(!(r == END_EDGE) && tl.target == r.source){
            for(auto tl : setL){
                res.push_back(OutEdge{tl.source,r.target});
            }
            r = right->produceNextEdge();
            std::cout << "******* JOIN => " << r;
        }
    }
    std::sort(res.begin(),res.end(), targetCompDesOut);

    ready = true;
    out.push(END_EDGE,true);
}

uint32_t MergeJoin::cost() const {
    return 0;
}

MergeJoin::MergeJoin(PhysicalOperator *left, PhysicalOperator *right) : PhysicalOperator(
        left, right, TARGET_SORTED), res() {}

std::ostream &MergeJoin::name(std::ostream &strm) const {
    return strm << "MergeJoin";
}


