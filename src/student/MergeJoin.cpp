//
// Created by Lucas Giordano on 17/03/2020.
//

#include "MergeJoin.h"

void removeDuplicate(std::vector<Edge>& array, Edge& last){
    while (!array.empty()&& last == array.back()){
        array.pop_back();
    }
}

Edge MergeJoin::produceNextEdge() {
    if (!ready) out.pop();
    removeDuplicate(res,lastEdgeProduced);
    removeDuplicate(extraOutput,lastEdgeProduced);
       if (!res.empty() && !extraOutput.empty()){
           if (!cmp(res.back(),extraOutput.back())) {
               lastEdgeProduced = res.back();
               res.pop_back();
           }
           else {
               lastEdgeProduced = extraOutput.back();
               extraOutput.pop_back();
           }
           return lastEdgeProduced;
       }
       else if (!res.empty()){
           lastEdgeProduced = res.back();
           res.pop_back();
           return lastEdgeProduced;
       }
       else if (!extraOutput.empty()){
           lastEdgeProduced = extraOutput.back();
           extraOutput.pop_back();
           return lastEdgeProduced;
       }
 /*   if (!res.empty()) {
        Edge r = res.back();
        res.pop_back();
        while (lastEdgeProduced == r && !res.empty()){
            r = res.back();
            res.pop_back();
        }
        bool isEnd = res.empty() && lastEdgeProduced == r;
        lastEdgeProduced = r;
        return (isEnd)?END_EDGE:r;
    }*/
    return END_EDGE;
}
/*cardStat MergeJoin::eval() {
    std::thread thdLeft([this] {
        left->evalPipeline(TARGET_SORTED);
    });
    std::thread thdRight([this] {
        right->evalPipeline(SOURCE_SORTED);
    });
    std::unordered_map<uint32_t,bool> hashIn;
    std::unordered_map<uint32_t,bool> hashOut;
    std::unordered_map<Edge,bool,HashEdge> hashEdge;

    Edge r = right->produceNextEdge();
    Edge l = left->produceNextEdge();
    std::vector<Edge> setL;
    while(r!=END_EDGE && l!=END_EDGE){
        while (l != END_EDGE && l.target < r.source) { //advance until we have a joining edge
            l = left->produceNextEdge();
        }
        setL.clear();
        Edge tl = l;
        bool done = false;
        while (!done && l!=END_EDGE){ //setL contains all edges in Left that have the same target value
            if (tl.target == l.target){
                setL.push_back(l);
                l = left->produceNextEdge();
            }
            else done = true;
        }
        while (r!=END_EDGE && r.source < tl.target) { //advance until we have a joining edge
            r = right->produceNextEdge();
        }
        while (!(r == END_EDGE) && tl.target == r.source){
            hashIn.insert({r.target,true});
            for(auto tl : setL){
                Edge joinedEdge = Edge{tl.source,r.target};
                hashOut.insert({tl.source,true});
                hashEdge.insert({joinedEdge,true});
            }
            r = right->produceNextEdge();
        }
    }
    left->terminate();
    right->terminate();
    thdLeft.join();
    thdRight.join();
    uint32_t noOut = hashOut.size();
    uint32_t noPath = hashEdge.size();
    uint32_t noIn = hashIn.size();
    return cardStat{noOut,noPath,noIn};
}*/

void MergeJoin::evalPipeline(ResultSorted resultSorted) {
    cmp = (resultSorted==TARGET_SORTED)?targetCompDesc:sourceCompDesc;
    std::thread thdLeft([this] {
        left->evalPipeline(TARGET_SORTED);
    });
    std::thread thdRight([this] {
        right->evalPipeline(SOURCE_SORTED);
    });
    std::thread* sortThd = nullptr;
    Edge r = right->produceNextEdge();
    Edge l = left->produceNextEdge();
    std::vector<Edge> setL;
    while(r!=END_EDGE && l!=END_EDGE){
        while (l != END_EDGE && l.target < r.source) { //advance until we have a joining edge
            l = left->produceNextEdge();
        }
        setL.clear();
        Edge tl = l;
        bool done = false;
        while (!done && l!=END_EDGE){ //setL contains all edges in Left that have the same target value
            if (tl.target == l.target){
                setL.push_back(l);
                l = left->produceNextEdge();
            }
            else done = true;
        }
        while (r!=END_EDGE && r.source < tl.target) { //advance until we have a joining edge
            r = right->produceNextEdge();
        }
        while (!(r == END_EDGE) && tl.target == r.source){
            for(auto tl : setL){
                if (getCardinality().noPaths > MIN_SIZE_THREAD && extraOutput.size() < getCardinality().noPaths/2){
                    extraOutput.push_back(Edge{tl.source,r.target});
                    if (extraOutput.size() == getCardinality().noPaths/2){
                        sortThd = new std::thread(([this, resultSorted] {
                            std::sort(extraOutput.begin(),extraOutput.end(), cmp);}));
                    }
                }
                else {
                    res.push_back(Edge{tl.source,r.target});
                }
            }
            r = right->produceNextEdge();
        }
    }
    left->terminate();
    right->terminate();
    thdLeft.join();
    thdRight.join();
    std::sort(res.begin(),res.end(), cmp);
    if (sortThd != nullptr) sortThd->join();
    ready = true;
    out.push(END_EDGE,true);
}

uint32_t MergeJoin::cost() {
    uint32_t  intermediateCost = left->cost() + right->cost();
    uint32_t  operatorCost = 0;
    uint32_t  leftCard = left->getCardinality().noPaths;
    uint32_t  rightCard = right->getCardinality().noPaths;
    operatorCost = leftCard + rightCard; //merge join cost if results are sorted;
    if (!(left->defaultResultSorted == TARGET_SORTED || left->defaultResultSorted == ANY)) { //add cost to sort results if needed
        operatorCost += leftCard* (uint32_t)std::log(leftCard);
    }
    if (!(right->defaultResultSorted == SOURCE_SORTED || right->defaultResultSorted == ANY)) {//add cost to sort results if needed
        operatorCost += rightCard* (uint32_t)std::log(rightCard);
    }
    return operatorCost+intermediateCost;
}

MergeJoin::MergeJoin(PhysicalOperator *left, PhysicalOperator *right) : PhysicalOperator(
        left, right,TARGET_SORTED), res() {}

std::ostream &MergeJoin::name(std::ostream &strm) const {
    return strm << "MergeJoin";
}