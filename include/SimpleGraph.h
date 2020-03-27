#ifndef QS_SIMPLEGRAPH_H
#define QS_SIMPLEGRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <regex>
#include <fstream>
#include "Graph.h"
#include<iterator> // for iterators
#include <map>
#include "Edge.h"


/**
 * Synopse that stores information related to path of length 1 in the graph, for each label
 * */
struct Syn1{
    uint32_t out    =0;  // * number of nodes in G which have outgoing edge labeled with l
    uint32_t in     =0;  // * number of nodes in G which have incoming edge labeled with l
    uint32_t path   =0;  // * number of paths in G labeled with l
    //uint32_t pairs  =0;  //number of distinct node pairs connected with paths labeled with l.
};
/**
 * Synopse that stores information related to path of length 2 in the graph, for each ordered
 * pair of labels l1 and l2
 * */
struct Syn2{
    //uint32_t out    =0;  //number of nodes in G which have outgoing path labeled with l1/l2
    uint32_t middle =0;  // * number of nodes in G which have incoming edge labeled l1 and outgoing edge labeled l2,
    uint32_t in     =0;  // * number of nodes in G which have incoming path labeled with l1/l2
    //uint32_t path   =0;  //number of paths in G labeled with l1/l2
    // uint32_t pairs  =0;  //number of distinct node pairs connected with paths labeled with l1/l2.
    //uint32_t one    =0;  //number of paths labeled l1 from nodes in out to nodes in middle, and
    uint32_t two    =0;  // * number of paths labeled l2 from nodes in middle to nodes in in.
};
/**
 * Synopse that stores information related to nodes that have both incoming/outgoing edges l1 and l2,
 * for each ordered pair of labels l1 and l2
 * */
struct Syn3{
    uint32_t middleIn  =0;  // * number of nodes in G which have incoming edge labeled l1 and incoming edge labeled l2,
    uint32_t in        =0;  // * number of nodes in G which have incoming path labeled with l1/~l2
    uint32_t twoIn     =0;  //* number of path from nodes in middleIn to nodes in syn3.in
};

struct Syn4{
    uint32_t middleOut = 0; // * number of nodes in G which have outgoing edge labeled l1 and outgoing edge labeled l2,
    uint32_t out       =0; // * number of nodes in G which have incoming path labeled with ~l1/l2
    uint32_t twoOut    =0; //* number of path from nodes in middleOut to nodes in syn4.out
};





//**************************************
typedef std::map<Edge,Edge> Map;
typedef std::map<Edge,Edge>::const_iterator  IndexIterator;
typedef std::pair<IndexIterator,IndexIterator> IndexResult;

class SimpleGraph : public Graph {
    
protected:
    uint32_t V=0;
    uint32_t L=0;
    std::vector<std::map<Edge,Edge>> mapSource;
    std::vector<std::map<Edge,Edge>> mapTarget;
    IndexResult getEdges(QueryEdge edgePrefix, const Map& map) const;
public:
    std::vector<Syn1> syn1;
    std::vector<std::vector<Syn2>> syn2;
    std::vector<std::vector<Syn3>> syn3;
    std::vector<std::vector<Syn4>> syn4;
    SimpleGraph() : mapSource(), mapTarget() {};
    ~SimpleGraph() = default;
    explicit SimpleGraph(uint32_t n);

    uint32_t getNoVertices() const override ;
    uint32_t getNoEdges() const override ;
    uint32_t getNoDistinctEdges() const override ;
    uint32_t getNoLabels() const override ;

    void addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) override ;
    void readFromContiguousFile(const std::string &fileName) override ;
    bool edgeExists(uint32_t from, uint32_t to, uint32_t edgeLabel);

    void setNoVertices(uint32_t n);
    void setNoLabels(uint32_t noLabels);

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
};

typedef SimpleGraph EdgeIndex;
#endif //QS_SIMPLEGRAPH_H
