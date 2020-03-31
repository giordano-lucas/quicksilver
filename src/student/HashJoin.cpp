//
// Created by Lucas Giordano on 28/03/2020.
//

#include "HashJoin.h"

uint32_t HashJoin::cost() {
    return left->getCardinality().noPaths + right->getCardinality().noPaths + left->cost() + right->cost();;
}

HashJoin::~HashJoin() {
    delete(left);
    delete(right);
}

bool HashJoin::isLeftBounded() const {
    return PhysicalOperator::isLeftBounded();
}

bool HashJoin::isRightBounded() const {
    return PhysicalOperator::isRightBounded();
}

void HashJoin::evalPipeline(ResultSorted resultSorted) {
    std::thread thdLeft([this] {
        left->evalPipeline(NOT_SORTED); /////change NOT_SORTED by other stuff if needed
    });
    std::thread thdRight([this] {
        right->evalPipeline(NOT_SORTED);//////change NOT_SORTED by other stuff if needed
    });
    ///// TO DO /////
    Edge r = right->produceNextEdge();
    Edge l = left->produceNextEdge();
    // main computation
    bool readFromLeft = true;
    std::unordered_multimap<Node, Node> leftMap;
    std::unordered_multimap<Node, Node> rightMap;

    while (r != END_EDGE && l != END_EDGE) {                                            //while there are still edges coming on both sides
        if (readFromLeft) {                                                             //if we are going to read from the left side next...
            leftMap.insert({l.target, l.source});                                       //insert source of edge l into the left hashmap with its target as the key
            auto range = rightMap.equal_range(l.target);                                //probe right map for all values with l.target as key
            for (auto it = range.first; it != range.second; ++it) {                     //iterate over all such values
                out.push(Edge{l.source, it->second}, false);             //push out the joined edge
            }
            l = left->produceNextEdge();                                                //and produce the next left edge
        } else {
            rightMap.insert({r.source, r.target});
            auto range = leftMap.equal_range(r.source);
            for (auto it = range.first; it != range.second; ++it) {
                out.push(Edge{it->second, r.target}, false);
            }
            r = right->produceNextEdge();
        }
        readFromLeft = !readFromLeft;                                                   //to alternate insertion and probe tables
    }
    while (l != END_EDGE) {
        leftMap.insert({l.target, l.source});
        auto range = rightMap.equal_range(l.target);
        for (auto it = range.first; it != range.second; ++it) {
            out.push(Edge{l.source, it->second}, false);
        }
        l = left->produceNextEdge();
    }
    while (r != END_EDGE) {
        //rightMap.insert({r.source, r.target});         //do not need to insert anymore, there will be no new left edges, so right map will never be probed
        auto range = leftMap.equal_range(r.source);
        for (auto it = range.first; it != range.second; ++it) {
            out.push(Edge{it->second, r.target}, false);
        }
        r = right->produceNextEdge();
    }


    /////END OF THE PHYSICAL OPERATOR => terminate children threads
    done = true;
    out.push(END_EDGE, true);
    left->terminate();
    right->terminate();
    thdLeft.join();
    thdRight.join();
    ///// SET DONE FLAG TO TRUE WHEN ALL EDGES HAVE BEEN CONSUMED BY PRODUCE NEXT EDGE (NOT NECESSARY IN THIS MEHTHOD)
}

Edge HashJoin::produceNextEdge() {
    if (!done || out.size() > 0) return out.pop();
    return END_EDGE;

}

std::ostream &HashJoin::name(std::ostream &strm) const {
    return strm << "HashJoin";
}
