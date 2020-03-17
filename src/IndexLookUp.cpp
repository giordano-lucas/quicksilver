//
// Created by Lucas Giordano on 12/03/2020.
//

#include "IndexLookUp.h"


IndexLookUp::IndexLookUp(EdgeIndex* index,Edge prefixEdge, bool reversed) :
        PhysicalOperator(nullptr, nullptr, SOURCE_SORTED),
        index(index), prefixEdge(prefixEdge), reversed(reversed), sortedRes(), res() {}

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
void IndexLookUp::evalPipeline() {
    if (prefixEdge.target == NONE && (prefixEdge.source != NONE || !reversed)) //choose the right sub EdgeIndex
         res = index->getEdgesSource(prefixEdge);                    // source sorted index access
    else res = index->getEdgesTarget(prefixEdge);                    // targed sorted index access
    if(( reversed && prefixEdge.source != NONE && prefixEdge.target == NONE) || // (n,l,*) & reversed
       (!reversed && prefixEdge.source == NONE && prefixEdge.target != NONE)) { // (*,l,n) & !reversed
                                                                                //  => reverse edges and resort
        for (IndexIterator it = res.first ; it != res.second ; ++it) sortedRes.push_back(reverse(it->first));
        res.first = res.second;
        std::sort(sortedRes.begin(),sortedRes.end(), sourceCompDesc);
    }
    ready = true;
    out.push(END_EDGE, true);
}

OutEdge IndexLookUp::produceNextEdge() {
    if (!ready) out.pop();
    if (sortedRes.empty() && res.first != res.second){
        Edge r = res.first->first;
        res.first++;
        return OutEdge{r.source,r.label};
    }
    else if (!sortedRes.empty()) {
        Edge r = sortedRes.back();
        sortedRes.pop_back();
        return OutEdge{r.source,r.label};
    }
    return END_EDGE;
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

std::ostream &IndexLookUp::name(std::ostream &strm) const {
    return strm << "IndexLookUp(" << prefixEdge << ")";
}


