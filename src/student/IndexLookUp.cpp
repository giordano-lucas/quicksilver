//
// Created by Lucas Giordano on 12/03/2020.
//

#include "IndexLookUp.h"


IndexLookUp::IndexLookUp(std::shared_ptr<SimpleGraph>& index,QueryEdge queryEdge, bool reversed) :
        PhysicalOperator(nullptr, nullptr,SOURCE_SORTED),
        index(index), queryEdge((reversed)?reverse(queryEdge):queryEdge), reversed(reversed), sortedResSource(),sortedResTarget(), res() {
    if (queryEdge.source != NONE) query.push_back(basic_query_t{select, queryEdge.source});
    query.push_back(basic_query_t{(reversed)?lower:greater, queryEdge.label});
    if (queryEdge.target != NONE) query.push_back(basic_query_t{select, queryEdge.target});

}

IndexLookUp::~IndexLookUp() {
}


/**
 * ======================== RESULT SORTED = SOURCE ==============================
 *
 * (*,l,*) & !reversed  => direct index access possible       (source)
 * (*,l,*) & reversed   => direct index access possible       (target)
 *
 * (n,l,*) & !reversed  => direct index access possible       (source)
 * (n,l,*) & reversed   => PROBLEM : need to reverse edges in (source) /!\ RESORT ON SOURCE
 *
 * (*,l,n) & !reversed  => PROLBEM : need to reverse edges in (target) /!\ RESORT ON SOURCE
 * (*,l,n) & reversed   => direct index access possible       (target)
 *
 * ======================== RESULT SORTED = TARGET ==============================
 *
 * (*,l,*) & !reversed  => direct index access possible       (source)  /!\ RESORT ON TARGET
 * (*,l,*) & reversed   => direct index access possible       (target)  /!\ RESORT ON TARGET
 *
 * (n,l,*) & !reversed  => direct index access possible       (source)  /!\ RESORT ON TARGET
 * (n,l,*) & reversed   => PROBLEM : need to reverse edges in (source)
 *
 * (*,l,n) & !reversed  => PROLBEM : need to reverse edges in (target)
 * (*,l,n) & reversed   => direct index access possible       (target)  /!\ RESORT ON TARGET
 *
 * ======================== RESULT SORTED = NONE ==============================
 *
 * (*,l,*) & !reversed  => direct index access possible       (source)
 * (*,l,*) & reversed   => direct index access possible       (target)
 *
 * (n,l,*) & !reversed  => direct index access possible       (source)
 * (n,l,*) & reversed   => PROBLEM : need to reverse edges in (source)
 *
 * (*,l,n) & !reversed  => PROLBEM : need to reverse edges in (target)
 * (*,l,n) & reversed   => direct index access possible       (target)
 *
 */
void IndexLookUp::evalPipeline(ResultSorted resultSorted) {
    if (resultSorted == TARGET_SORTED && queryEdge.source == NONE && queryEdge.target == NONE){
        resValid = false;
        if (!reversed) res = index->getEdgesTarget(queryEdge);
        else           res = index->getEdgesSource(queryEdge);
        for (IndexIterator it = res.first ; it != res.second ; ++it)  sortedResTarget.push_back(reverse(it->first));
        ready = true;
        out.push(END_EDGE, true);
        return;
    }

    if (queryEdge.target == NONE && (queryEdge.source != NONE || !reversed)) //choose the right sub EdgeIndex
        res = index->getEdgesSource(queryEdge);                     // source sorted index access
    else res = index->getEdgesTarget(queryEdge);                    // targed sorted index access

    if(( reversed && queryEdge.source != NONE && queryEdge.target == NONE) || // (n,l,*) & reversed
       (!reversed && queryEdge.source == NONE && queryEdge.target != NONE)) { // (*,l,n) & !reversed
                                                                                //  => reverse edges and resort
        for (IndexIterator it = res.first ; it != res.second ; ++it) sortedResSource.push_back(reverse(it->first));
        resValid = false;
        if (resultSorted == SOURCE_SORTED)  //only need to sort on source if requested
            std::sort(sortedResSource.begin(),sortedResSource.end(), sourceCompDesc);
    } else if (resultSorted == TARGET_SORTED){
        for (IndexIterator it = res.first ; it != res.second ; ++it) sortedResTarget.push_back(it->first);
        resValid = false;
        std::sort(sortedResTarget.begin(),sortedResTarget.end(), targetCompDesc);
    }
    ready = true;
    out.push(END_EDGE, true);
}

Edge IndexLookUp::produceNextEdge() {
    if (!ready) out.pop();

    if (!sortedResTarget.empty() && i < sortedResTarget.size() && queryEdge.target == NONE && queryEdge.source == NONE){
        Edge r = sortedResTarget[i];
        i++;
        return r;
    }

    if (resValid && res.first != res.second){
        Edge r = res.first->first;
        res.first++;
        return r;
    }
    else if (!sortedResSource.empty()) {
        Edge r = sortedResSource.back();
        sortedResSource.pop_back();
        return r;
    }
    else if (i!= sortedResTarget.size() && !sortedResTarget.empty()){
        Edge r = sortedResTarget.back();
        sortedResTarget.pop_back();
        return r;
    }
    return END_EDGE;
}

uint32_t IndexLookUp::cost() const {
    return 1;
}

bool IndexLookUp::isLeftBounded() const {
    return ( reversed && queryEdge.target != NONE) || (!reversed && queryEdge.source != NONE);
}

bool IndexLookUp::isRightBounded() const {
    return ( reversed && queryEdge.source != NONE) || (!reversed && queryEdge.target != NONE);
}

std::ostream &IndexLookUp::name(std::ostream &strm) const {
    return strm << "IndexLookUp : " << reversed << "|"<< queryEdge.label;
}


