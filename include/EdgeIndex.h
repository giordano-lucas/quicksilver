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

typedef std::map<Edge,Edge> Map;
typedef std::map<Edge,Edge>::const_iterator  IndexIterator;
typedef std::pair<IndexIterator,IndexIterator> IndexResult;
/*
 * If (*,l,*) return regular edges
 * if (n,l,*) return regular edges
 * if (*,l,n) return reversed edges
 *
 * */
class EdgeIndex {
private:
    uint32_t V=0;
    uint32_t L=0;
    std::map<Edge,Edge> mapSource;
    std::map<Edge,Edge> mapTarget;
    Edge nextIncrementalEdge(Edge& e) const;
    IndexResult getEdges(Edge edgePrefix, const Map& map) const;
    Edge toPrefix(Edge e) const;
public:
    /*Constructor*/
    EdgeIndex() : mapSource(), mapTarget() {};
    /*Access methods*/
    /**
     * Returns iterator on edges {e=(s,l,t) | e=(s,l,t) in G} and SORTED :
     *          First  => BY LABEL
     *          Then   => BY SOURCE
     *          FINALY => BY TARGET
     **/
    IndexResult getEdgesSource(Edge edgePrefix) const;
    /**
     * Returns iterator on  REVERSED edges {e=(t,l,s) | e=(s,l,t) in G} and SORTED :
     *          First  => BY LABEL
     *          Then   => BY TARGET
     *          FINALY => BY SOURCE
     **/
    IndexResult getEdgesTarget(Edge edgePrefix) const;
    /**
     * Returns iterator on all edges in the index (or reversed edges if reversed == true)
     **/
    IndexResult allEdges(bool reversed) const;
    /*Insertion methods*/
    void insert(Edge e);
    /**
     * Insert all edges in the index
     * */
    void insertAll(std::vector<Edge> edges);
    /**
     * Construct the index from the file of name fileName
     **/
    void buildFromFile(const std::string &fileName);
};

/**
 * Redefinition of << operator to easily debug
 **/
std::ostream& operator<<(std::ostream &strm, const EdgeIndex &index);
#endif //QUICKSILVER_EDGEINDEX_H
