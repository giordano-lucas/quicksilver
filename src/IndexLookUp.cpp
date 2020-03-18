//
// Created by Lucas Giordano on 12/03/2020.
//

#include "IndexLookUp.h"


IndexLookUp::IndexLookUp(EdgeIndex* index,Edge prefixEdge, bool reversed,ResultSorted resultSorted) :
        PhysicalOperator(nullptr, nullptr, resultSorted),
        index(index), prefixEdge((reversed)?reverse(prefixEdge):prefixEdge), reversed(reversed), sortedResSource(),sortedResTarget(), res() {
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
void IndexLookUp::evalPipeline() {

    if (prefixEdge.target == NONE && (prefixEdge.source != NONE || !reversed)) //choose the right sub EdgeIndex
        res = index->getEdgesSource(prefixEdge);                     // source sorted index access
    else res = index->getEdgesTarget(prefixEdge);                    // targed sorted index access

    if(( reversed && prefixEdge.source != NONE && prefixEdge.target == NONE) || // (n,l,*) & reversed
       (!reversed && prefixEdge.source == NONE && prefixEdge.target != NONE)) { // (*,l,n) & !reversed
                                                                                //  => reverse edges and resort
        for (IndexIterator it = res.first ; it != res.second ; ++it) sortedResSource.push_back(reverse(it->first));
        resValid = false;
        if (resultSorted == SOURCE_SORTED)  //only need to sort on source if requested
            std::sort(sortedResSource.begin(),sortedResSource.end(), sourceCompDesc);
    } else if (resultSorted == TARGET_SORTED){
        for (IndexIterator it = res.first ; it != res.second ; ++it) sortedResTarget.push_back(reverse(it->first));
        resValid = false;
        std::sort(sortedResTarget.begin(),sortedResTarget.end(), targetCompDesc);
    }
    ready = true;
    out.push(END_EDGE, true);
}

OutEdge IndexLookUp::produceNextEdge() {
    if (!ready) out.pop();
    if (resValid && res.first != res.second){
        Edge r = res.first->first;
        res.first++;
        return OutEdge{r.source,r.target};
    }
    else if (!sortedResSource.empty()) {
        Edge r = sortedResSource.back();
        sortedResSource.pop_back();
        return OutEdge{r.source,r.target};
    }
    else if (!sortedResTarget.empty()){
        Edge r = sortedResTarget.back();
        sortedResTarget.pop_back();
        return OutEdge{r.source,r.target};
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
    return strm << "IndexLookUp : " << reversed << "|"<< prefixEdge;
}


