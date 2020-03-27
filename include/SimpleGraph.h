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
#include "Synopse.h"

//**************************************
#include "Edge.h"
#include "Synopse.h"
#include "Estimator.h"
#include "Graph.h"
#include <regex>
#include <fstream>
#include <unordered_map>

//***************** DATA STRUCTURES *****************
typedef struct{
    Node source;
    uint16_t nbTargets;
    uint32_t indexEdges;
} Header;
typedef struct{
    Header* headers;
    uint32_t nbHeaders;
    Node* edges;
    uint32_t nbEdges;
} LabelIndex;


typedef LabelIndex* Index;
//********************** CLASS **********************
class SimpleGraph : public Graph {
    ///// =============== Nested sub class => iterator ================
public:
    class Iterator {
    private:
        LabelIndex* index;
        Header* source;
        Header* end;
        Node*   target;
        uint16_t remainingTargets;
        bool done        = false;
        bool needReverse = false;
        // Sorted iterator variables
        bool needSorted  = false;
        Edge* sortedArray     = nullptr;
        Edge* allocatedArray     = nullptr;
        Edge* itSorted   = nullptr;
    public:
        Iterator(LabelIndex* index, Header* start, bool needReverse);
        Iterator(LabelIndex* index, Header* start, Header* end, bool needReverse);
        Iterator();
        Iterator sort(Comparator cmp);
        Iterator operator++();
        Edge operator*();
        Iterator skip();
        bool hasNext() const;
    };
    //// ========================= Real class =========================
private:
    uint32_t V=0;
    uint32_t L=0;
    Index sourceIndex; //array of L subIndex
    Index targetIndex; // array of L subIndex
    Header* find(Index index, QueryEdge queryEdge) const;
    void insertAll(std::vector<Edge> &edges, Label l, bool reversed, Comparator cmp, Index index);
    Iterator getEdges(Index index, QueryEdge queryEdge, bool needReverse) const;
public:
    ~SimpleGraph(); //destructor
    Iterator getEdgesSource(QueryEdge queryEdge, bool needReverse) const;
    Iterator getEdgesTarget(QueryEdge queryEdge, bool needReverse) const;
    Iterator getEdgesSource(QueryEdge queryEdge) const;
    Iterator getEdgesTarget(QueryEdge queryEdge) const;
    /*Insertion methods*/
    void insertAll(std::vector<Edge> &edges, Label l);
    void readFromContiguousFile(const std::string &fileName);

    uint32_t getNoVertices() const override;

    uint32_t getNoEdges() const override;

    uint32_t getNoDistinctEdges() const override;

    uint32_t getNoLabels() const override;

    void addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) override;

    //// *** Synposes ***
    std::vector<Syn1> syn1;
    std::vector<std::vector<Syn2>> syn2;
    std::vector<std::vector<Syn3>> syn3;
    std::vector<std::vector<Syn4>> syn4;
};
typedef SimpleGraph::Iterator IndexIterator;
#endif //QS_SIMPLEGRAPH_H
