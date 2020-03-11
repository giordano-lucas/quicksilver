//
// Created by Lucas Giordano on 11/03/2020.
//

#ifndef QUICKSILVER_EDGE_H
#define QUICKSILVER_EDGE_H
#include <iostream>
#include <vector>

typedef uint32_t Node;
typedef uint32_t Label;
#define NONE -1
typedef struct {
    Node source : 32;
    Label label : 32;
    Node target : 32;
} Edge;
    bool labelSourceComp(const Edge& a, const Edge& b);
    bool labelTargetComp(const Edge& a, const Edge& b);
    bool sourceComp(const Edge& a, const Edge& b);
    bool targetComp(const Edge& a, const Edge& b);
    Edge reverse(const Edge& a);
    std::ostream& operator<<(std::ostream &strm, const Edge &e);
    bool operator<(const Edge& a, const Edge& b);

#endif //QUICKSILVER_EDGE_H
