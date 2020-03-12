//
// Created by Lucas Giordano on 12/03/2020.
//

#include "IndexLookUp.h"


IndexLookUp::IndexLookUp(Queue *out,  std::mutex *mutexOut, EdgeIndex* index,Edge prefixEdge, bool reversed) :
        PhysicalOperator(out, nullptr, nullptr, mutexOut, nullptr, nullptr, nullptr, nullptr),
        index(index), prefixEdge(prefixEdge), reversed(reversed) {}

IndexLookUp::~IndexLookUp() {
}


/**
 * (*,l,*) & !reversed  => direct index access possible       (source)
 * (*,l,*) & reversed   => direct index access possible       (target)
 *
 * (n,l,*) & !reversed  => direct index access possible       (source)
 * (n,l,*) & reversed   => PROBLEM : need to reverse edges in (source) and resort
 *
 * (*,l,n) & !reversed  => PROLBEM : need to reverse edges in (target) and resort
 * (*,l,n) & reversed   => direct index access possible       (target)
 */
void IndexLookUp::evalPipeline() const {
    IndexResult res;
    if (prefixEdge.target == NONE && (prefixEdge.source != NONE || !reversed)) //choose the right sub EdgeIndex
         res = index->getEdgesSource(prefixEdge);                    // source sorted index access
    else res = index->getEdgesTarget(prefixEdge);                    // targed sorted index access

    if(( reversed && prefixEdge.source != NONE && prefixEdge.target == NONE) || // (n,l,*) & reversed
       (!reversed && prefixEdge.source == NONE && prefixEdge.target != NONE)) { // (*,l,n) & !reversed
                                                                                //  => reverse edges and resort
        std::vector<Edge> toSort;
        for (IndexIterator it = res.first ; it != res.second ; ++it) toSort.push_back(reverse(it->first));
        std::sort(toSort.begin(),toSort.end(), sourceComp);
        for (std::vector<Edge>::iterator it = toSort.begin(); it != toSort.end() ; ++it) push(out,mutexOut,new Edge(*it));
    }
    else for (IndexIterator it = res.first ; it != res.second ; ++it) push(out, mutexOut,new Edge(it->first));

    //add END EDGE 
    push(out,mutexOut,new Edge(END_EDGE));
}

uint32_t IndexLookUp::cost() const {
    return 1;
}

bool IndexLookUp::isLeftBounded() const {
    return ( reversed && prefixEdge.target != NONE) || (!reversed && prefixEdge.source != NONE);
}

bool IndexLookUp::isRightBounded() const {
    return ( reversed && prefixEdge.source != NONE) || (!reversed && prefixEdge.target != NONE);
}


