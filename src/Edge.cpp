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
    if(e.label==NONE)  strm << "(" << e.source << ")-"            << "->(" << e.target <<") \n";
    else               strm << "(" << e.source << ")-" << e.label << "->(" << e.target <<") \n";
    return strm;
}

bool labelSourceComp(const Edge &a, const Edge &b) {
    return (a.label != b.label)? a.label<b.label: sourceComp(a,b);
}

bool labelTargetComp(const Edge &a, const Edge &b) {
    return (a.label != b.label)? a.label<b.label: targetComp(a,b);
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

bool operator<(const Edge &a, const Edge &b) {
    return labelSourceComp(a,b);
}

Edge reverse(const Edge &a) {
    return Edge{a.target,a.label,a.source};
}

bool operator==(const Edge &a, const Edge &b) {
    return a.source == b.source && a.label == b.label && a.target == b.target;
}

//*****************************************************************************
bool operator==(const OutEdge &a, const OutEdge &b) {
    return a.source == b.source && a.target == b.target;
}