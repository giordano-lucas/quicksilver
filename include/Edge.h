//
// Created by Lucas Giordano on 11/03/2020.
//

#ifndef QUICKSILVER_EDGE_H
#define QUICKSILVER_EDGE_H
#include <iostream>
#include <vector>
#include <functional>
typedef uint32_t Node;
typedef uint32_t Label;
#define NONE UINT32_MAX  //*********************************** TO BE CHANGED ********************************

// ============================ Prefix Edges =============================
typedef struct {
    Node source ;
    Label label ;
    Node target ;
} Edge;
    bool labelSourceComp(const Edge& a, const Edge& b);
    bool labelTargetComp(const Edge& a, const Edge& b);
    bool sourceComp(const Edge& a, const Edge& b);
    bool sourceCompDesc(const Edge& a, const Edge& b);
    bool targetComp(const Edge& a, const Edge& b);
    Edge reverse(const Edge& a);
    std::ostream& operator<<(std::ostream &strm, const Edge &e);
    bool operator==(const Edge& a, const Edge& b);
    bool operator<(const Edge& a, const Edge& b);

// ============================ Output Edges =============================
typedef struct {
    Node source ;
    Node target ;
} OutEdge;

bool operator==(const OutEdge& a, const OutEdge& b);
class HashOutEdge {
public:
    size_t operator()(const OutEdge& e) const{
        std::hash<uint32_t> hash;
        return hash(hash(e.source) + e.target);
    }
};

#endif //QUICKSILVER_EDGE_H
