//
// Created by Lucas Giordano on 27/03/2020.
//

#ifndef QUICKSILVER_SMARTGRAPH_H
#define QUICKSILVER_SMARTGRAPH_H

#include "Edge.h"
#include "Synopse.h"
#include "Estimator.h"
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
} LabelIndex;
typedef LabelIndex* Index;
//********************** CLASS **********************
class SmartGraph {
    ///// =============== Nested sub class => iterator ================
public:
    class Iterator{
    private:
        LabelIndex* index;
        Header* source;
        Header* end;
        Node*   target;
        uint16_t remainingTargets;
        bool done = false;
        bool needReverse;
    public:
        Iterator(LabelIndex* index, Header* start, bool needReverse);
        Iterator(LabelIndex* index, Header* start, Header* end, bool needReverse);
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
    ~SmartGraph(); //destructor
    Iterator getEdgesSource(QueryEdge queryEdge, bool needReverse) const;
    Iterator getEdgesTarget(QueryEdge queryEdge, bool needReverse) const;
    Iterator getEdgesSource(QueryEdge queryEdge) const;
    Iterator getEdgesTarget(QueryEdge queryEdge) const;
    /*Insertion methods*/
    void insertAll(std::vector<Edge> &edges, Label l);
    void readFromContiguousFile(const std::string &fileName);
    //// *** Synposes ***
    std::vector<Syn1> syn1;
    std::vector<std::vector<Syn2>> syn2;
    std::vector<std::vector<Syn3>> syn3;
    std::vector<std::vector<Syn4>> syn4;
};
typedef SmartGraph::Iterator IndexIterator;

#endif //QUICKSILVER_SMARTGRAPH_H