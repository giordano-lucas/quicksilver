//
// Created by Lucas Giordano on 17/03/2020.
//

#include "MergeJoin.h"

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
    std::thread thdLeft([this] {
        left->evalPipeline(TARGET_SORTED);
    });
    std::thread thdRight([this] {
        right->evalPipeline(SOURCE_SORTED);
    });
    MergeJoin::resultSorted = resultSorted;
    Edge r = right->produceNextEdge();
    Edge l = left->produceNextEdge();
    std::vector<Edge> setL;
    while(r!=END_EDGE && l!=END_EDGE){
        while (l != END_EDGE && l.target < r.source) { //advance until we have a joining edge
            left->skip(r.source);
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
            right->skip(tl.target);
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
   // std::sort(res.begin(),res.end(), (resultSorted==TARGET_SORTED)?targetCompDesc:sourceCompDesc);
    std::sort(res.begin(),res.end(), (resultSorted==TARGET_SORTED)?targetComp:sourceComp);
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


void MergeJoin::removeDuplicate(std::vector<Edge>& array, Edge& last){
    while ( idx < array.size() && last == array[idx]){
        idx++;
    }
}

Edge MergeJoin::produceNextEdge() {
    if (!ready) out.pop();
    removeDuplicate(res,lastEdgeProduced);
    if (idx < res.size() ) {
        lastEdgeProduced = res[idx];
        return lastEdgeProduced;
    }
    return END_EDGE;
}

#define get(e) ((resultSorted == SOURCE_SORTED)? (e).source : (e).target)
size_t MergeJoin::binarySearch(std::vector<Edge>& arr, size_t  low, Node key){
    size_t  high = arr.size()-1;
    size_t ans = -1;

    while (low <= high) {
        int mid = low + (high - low + 1) / 2;
        Node midVal = get(arr[mid]);

        if (midVal < key) {

            // if mid is less than key, all elements
            // in range [low, mid - 1] are < key
            // we note down the last found index, then
            // we search in right side of mid
            // so we now search in [mid + 1, high]
            ans = mid;
            low = mid + 1;
        }
        else if (midVal > key) {

            // if mid is greater than key, all elements
            // in range [mid + 1, high] are > key
            // then we search in left side of mid
            // so we now search in [low, mid - 1]
            high = mid - 1;
        }
        else if (midVal == key) {

            // if mid is equal to key, all elements
            // in range [mid + 1, high] are >= key
            // then we search in left side of mid
            // so we now search in [low, mid - 1]
            high = mid - 1;
        }
    }
    return ans + 1;
}

void MergeJoin::skip(Node until) {
    if (idx >= res.size()) return;
    size_t nextSource = binarySearch(res,idx, until);
  //  assert(idx >= 0 && idx < res.size());
    idx = nextSource;
}



