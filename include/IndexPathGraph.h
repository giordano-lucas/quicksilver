//
// Created by Lucas Giordano on 07/03/2020.
//

#ifndef QUICKSILVER_INDEXPATHGRAPH_H
#define QUICKSILVER_INDEXPATHGRAPH_H

#include "IndexPath.h"
#include <regex>
#include <fstream>
class IndexPathGraph : public Graph {
private:
    uint32_t V;
    uint32_t L;
    IndexPath index;
public:
    IndexPathGraph(): index(), V(0), L(0) {}
    uint32_t getNoVertices() const ;
    uint32_t getNoEdges() const;
    uint32_t getNoDistinctEdges() const;
    uint32_t getNoLabels() const;
    void addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel);
    void readFromContiguousFile(const std::string &fileName);
};
#endif //QUICKSILVER_INDEXPATHGRAPH_H
