//
// Created by Lucas Giordano on 12/03/2020.
//

#include "IndexLookUp.h"

cardStat IndexLookUp::eval() const {
    return cardStat();
}

void IndexLookUp::evalPipeline() const {
    IndexResult res = index->getEdges(prefixEdge);

    if ((prefixEdge.target != NONE && !reversed) || (prefixEdge.source != NONE && reversed))
        for (IndexIterator it = res.first ; it != res.second ; ++it){
            if (!reversed) out->push(new Edge(it->first));
        }
    else { //sort res
        std::vector<Edge> toSort;
        for (IndexIterator it = res.first ; it != res.second ; ++it) toSort.push_back(it->first);

        if(!reversed) std::sort(toSort.begin(),toSort.end(), sourceComp);

    }
}

uint32_t IndexLookUp::cost() const {
    return 1;
}

IndexLookUp::IndexLookUp(Queue *out,  std::mutex *mutexOut, EdgeIndex* index,Edge prefixEdge, bool reversed) :
        PhysicalOperator(out, nullptr, nullptr, mutexOut, nullptr, nullptr, nullptr, nullptr),
        index(index), prefixEdge(prefixEdge), reversed(reversed) {}

IndexLookUp::~IndexLookUp() {

}

