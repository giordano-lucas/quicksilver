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

class EdgeIndex {
private:
    uint32_t V=0;
    uint32_t L=0;
    std::vector<std::map<Edge,Edge>> mapSource;
    std::vector<std::map<Edge,Edge>> mapTarget;
    IndexResult getEdges(QueryEdge edgePrefix, const Map& map) const;
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
    IndexResult getEdgesSource(QueryEdge edgePrefix) const;
    /**
     * Returns iterator on  REVERSED edges {e=(t,l,s) | e=(s,l,t) in G} and SORTED :
     *          First  => BY LABEL
     *          Then   => BY TARGET
     *          FINALY => BY SOURCE
     **/
    IndexResult getEdgesTarget(QueryEdge edgePrefix) const;
    /*Insertion methods*/
    void insert(Edge e,Node l);
    /**
     * Insert all edges in the index
     * */
    void insertAll(std::vector<Edge> edges, Label l);
    /**
     * Insert all edges in the index
     * */
    void insertAll(std::vector<QueryEdge> edges);
    /**
     * Construct the index from the file of name fileName
     **/
    void buildFromFile(const std::string &fileName);
};

#endif //QUICKSILVER_EDGEINDEX_H
