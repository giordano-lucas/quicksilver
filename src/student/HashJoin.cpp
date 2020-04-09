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

void HashJoin::evalPipeline(ResultSorted resultSorted) {
    std::thread thdLeft([this] {
        left->evalPipeline(NOT_SORTED); /////change NOT_SORTED by other stuff if needed
    });
   // std::thread thdRight([this] {
      right->evalPipeline(NOT_SORTED);//////change NOT_SORTED by other stuff if needed
  //  });

    ///// TO DO /////

    std::unordered_multimap<Node, Node> rightMap;


    for (Edge r = right->produceNextEdge(); r != END_EDGE ; r = right->produceNextEdge()){
        rightMap.insert({r.source,r.target});
    }

    for (Edge l = left->produceNextEdge(); l != END_EDGE ; l = left->produceNextEdge()){
        auto range = rightMap.equal_range(l.target);
        for (auto it = range.first; it != range.second; ++it) {//iterate over all such values
            Edge outE = Edge{l.source, it->second};
            if (cache.find(outE) != cache.end()) {
                cache.insert({outE,true});
                out.push(Edge{l.source, it->second}, false);             //push out the joined edge
            }

        }
    }

    /////END OF THE PHYSICAL OPERATOR => terminate children threads
    done = true;
    out.push(END_EDGE, true);
    left->terminate();
    right->terminate();
    thdLeft.join();
   // thdRight.join();
    ///// SET DONE FLAG TO TRUE WHEN ALL EDGES HAVE BEEN CONSUMED BY PRODUCE NEXT EDGE (NOT NECESSARY IN THIS MEHTHOD)
}

Edge HashJoin::produceNextEdge() {
    if (!done || out.size() > 0) return out.pop();
    return END_EDGE;

}

std::ostream &HashJoin::name(std::ostream &strm) const {
    return strm << "HashJoin";
}
