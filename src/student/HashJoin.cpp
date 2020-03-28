//
// Created by Lucas Giordano on 28/03/2020.
//

#include "HashJoin.h"

uint32_t HashJoin::cost() const {
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
    //......
    //......
    //......
    /////END OF THE PHYSICAL OPERATOR => terminate children threads
    left->terminate();
    right->terminate();
    thdLeft.join();
    thdRight.join();
    ///// SET DONE FLAG TO TRUE WHEN ALL EDGES HAVE BEEN CONSUMED BY PRODUCE NEXT EDGE (NOT NECESSARY IN THIS MEHTHOD)
}

Edge HashJoin::produceNextEdge() {
    if (!done) return out.pop();
    return END_EDGE; //// ALWAYS return END EDGE WHEN ALL EDGES HAVE BEEN PRODUCED
}

std::ostream &HashJoin::name(std::ostream &strm) const {
    return strm << "HashJoin";
}
