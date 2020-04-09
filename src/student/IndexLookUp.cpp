//
// Created by Lucas Giordano on 12/03/2020.
//

#include "IndexLookUp.h"


IndexLookUp::IndexLookUp(std::shared_ptr<SimpleGraph>& index,QueryEdge queryEdge, bool reversed) :
        PhysicalOperator(nullptr, nullptr,ANY),
        index(index), queryEdge(queryEdge), reversed(reversed),res() {
    if (queryEdge.source != NONE) query.push_back(basic_query_t{selection, queryEdge.source});
    query.push_back(basic_query_t{(reversed)?lower:greater, queryEdge.label});
    if (queryEdge.target != NONE) query.push_back(basic_query_t{selection, queryEdge.target});

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
 * (*,l,*) & !reversed  => direct index access possible       (target)  (=>reverse)
 * (*,l,*) & reversed   => direct index access possible       (soure)   (=>reverse)
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
    IndexLookUp::resultSorted = resultSorted;

    if (queryEdge.source == NONE && queryEdge.target == NONE){
        switch(resultSorted){
            case TARGET_SORTED: res = (reversed)?index->getEdgesSource(queryEdge,true):index->getEdgesTarget(queryEdge,true);break;
            case ANY          :
            case NOT_SORTED   : //go in SourceSorted
            case SOURCE_SORTED: res = (reversed)?index->getEdgesTarget(queryEdge):index->getEdgesSource(queryEdge);break;
        }
    }
    else if (queryEdge.source != NONE){
        res = (reversed)?index->getEdgesSource(queryEdge,true):index->getEdgesSource(queryEdge);
    }
    else {
        res = (reversed)?index->getEdgesTarget(queryEdge):index->getEdgesTarget(queryEdge,true);
    }
    ready = true;
    Syn1 syn = index->syn1[queryEdge.label];
    if (!reversed) outCardStat = cardStat{syn.out,syn.path,syn.in};
    else           outCardStat = cardStat{syn.in,syn.path,syn.out};

    out.push(END_EDGE, true);

}

Edge IndexLookUp::produceNextEdge() {
    if (!ready) out.pop();
    if (res.hasNext()) {
        Edge r = *res;
        ++res;
        return r;
    }
    return END_EDGE;
}

uint32_t IndexLookUp::cost() {
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

#define get(e) ((resultSorted == SOURCE_SORTED)? (e).source : (e).target)
void IndexLookUp::skip(Node until) {
    while ( get((*res)) < until) res.skip();
}


