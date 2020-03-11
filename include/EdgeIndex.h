//
// Created by Lucas Giordano on 11/03/2020.
//

#ifndef QUICKSILVER_EDGEINDEX_H
#define QUICKSILVER_EDGEINDEX_H
#include<iterator> // for iterators
#include <map>
#include "Edge.h"
#include "Graph.h"
#include <regex>
#include <fstream>

typedef std::map<Edge,Edge>::const_iterator  IndexIterator;
typedef std::pair<IndexIterator,IndexIterator> IndexResult;

class EdgeIndex {
private:
    uint32_t V=0;
    uint32_t L=0;
    std::map<Edge,Edge> mapSource;
    std::map<Edge,Edge> mapTarget;
    Edge nextIncrementalEdge(Edge& e,bool reversed) const;
public:
    /*Constructor*/
    EdgeIndex() : mapSource(), mapTarget() {};
    /*Access methods*/
    IndexResult getEdges(Edge edgePrefix) const;
    IndexResult allEdges(bool reversed) const;
    /*Insertion methods*/
    void insert(Edge e);
    void insertAll(std::vector<Edge> edges);
    void buildFromFile(const std::string &fileName);
};
std::ostream& operator<<(std::ostream &strm, const EdgeIndex &index);
#endif //QUICKSILVER_EDGEINDEX_H
