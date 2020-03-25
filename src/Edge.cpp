//
// Created by Lucas Giordano on 11/03/2020.
//

#include <functional>
#include <algorithm>
#include <Edge.h>

/*
 * Overloading of << operator (toString)
 * */
std::ostream &operator<<(std::ostream &strm, const Edge &e) {
    strm << "(" << e.source << ")-" << "->(" << e.target <<") \n";
    return strm;
}
QueryEdge reverse(const QueryEdge &a) {
    return QueryEdge{a.target,a.label,a.source};
}

bool sourceComp(const Edge &a, const Edge &b) {
    return (a.source == b.source)?a.target<b.target:a.source<b.source;
}
bool sourceCompDesc(const Edge &a, const Edge &b) {
    return (a.source == b.source)?a.target>b.target:a.source>b.source;
}

bool targetComp(const Edge &a, const Edge &b) {
    return (a.target == b.target)?a.source<b.source:a.target<b.target;
}
bool targetCompDesc(const Edge &a, const Edge &b) {
    return (a.target == b.target)?a.source>b.source:a.target>b.target;
}

bool operator<(const Edge &a, const Edge &b) {
    return sourceComp(a,b);
}

Edge reverse(const Edge &a) {
    return Edge{a.target,a.source};
}

bool operator==(const Edge &a, const Edge &b) {
    return a.source == b.source && a.target == b.target;
}

bool labelSourceComp(const QueryEdge &a, const QueryEdge &b) {
    return (a.label != b.label)? a.label < b.label: ((a.source != b.source)?a.source < b.source : a.target < b.target);
}

bool labelTargetComp(const QueryEdge &a, const QueryEdge &b) {
    return (a.label != b.label)? a.label < b.label: ((a.target != b.target)?a.target < b.target : a.source < b.source);
}
bool operator!=(const Edge& a, const Edge& b) {return !(a==b);};