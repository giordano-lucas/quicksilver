//
// Created by Lucas Giordano on 27/03/2020.
//

#include "SimpleGraph.h"
#include <string.h>

//****************************************************
//******************* Destructor *********************
//****************************************************

SimpleGraph::~SimpleGraph()  {
    for (Label l= 0 ; l < L ; ++l){
        free(sourceIndex[l].edges);
        free(sourceIndex[l].headers);
        free(targetIndex[l].edges);
        free(targetIndex[l].headers);
    }
    free(sourceIndex);
    free(targetIndex);
}

//****************************************************
//***************** Construction *********************
//****************************************************

void SimpleGraph::readFromContiguousFile(const std::string &fileName) {
    std::string line;
    std::ifstream graphFile { fileName };

    std::regex edgePat (R"((\d+)\s(\d+)\s(\d+)\s\.)"); // subject predicate object .
    std::regex headerPat (R"((\d+),(\d+),(\d+))"); // noNodes,noEdges,noLabels

    // parse the header (1st line)
    std::getline(graphFile, line);
    std::smatch matches;
    if(std::regex_search(line, matches, headerPat)) {
        uint32_t noNodes  = (uint32_t) std::stoul(matches[1]);
        uint32_t noLabels = (uint32_t) std::stoul(matches[3]);

        V = noNodes;
        L = noLabels;
        /////*********************** Allocation *******************************
        sourceIndex = static_cast<Index>(calloc(L, sizeof(LabelIndex)));
        targetIndex = static_cast<Index>(calloc(L, sizeof(LabelIndex)));
        ///// -----------------------------------------------------------------
        randomAccessTableSource= static_cast<RandomAccessTable*>(calloc(L, sizeof(RandomAccessTable)));
        randomAccessTableTarget= static_cast<RandomAccessTable*>(calloc(L, sizeof(RandomAccessTable)));
        /////***************************** END ********************************
    } else {
        throw std::runtime_error(std::string("Invalid graph header!"));
    }
    std::vector<std::vector<Edge>> edges(L, std::vector<Edge>());
    //set syn1
    syn1.resize(L);
    std::vector<std::unordered_map<Node,bool>> inMap(L, std::unordered_map<Node,bool>());
    std::vector<std::unordered_map<Node,bool>> outMap(L, std::unordered_map<Node,bool>());
    std::unordered_map<Label,bool> labelMap;
    ////////////////////////////// L2 INDEX /////
    std::vector<std::vector<Edge>> adj(V, std::vector<Edge>());
    sourceIndex2 = static_cast<Index*>(calloc(L, sizeof(Index)));
   // randomAccessTableSource2= static_cast<RandomAccessTable**>(calloc(L, sizeof(RandomAccessTable*)));
    for (auto l = 0 ; l < L ;++l){
        sourceIndex2[l] = static_cast<Index>(calloc(L, sizeof(LabelIndex)));
       // randomAccessTableSource2[l]= static_cast<RandomAccessTable*>(calloc(L, sizeof(RandomAccessTable)));
    }
    /////////////////////////////

    while(std::getline(graphFile, line)) {

        if(std::regex_search(line, matches, edgePat)) {
            uint32_t subject = (uint32_t) std::stoul(matches[1]);
            uint32_t predicate = (uint32_t) std::stoul(matches[2]);
            uint32_t object = (uint32_t) std::stoul(matches[3]);
            edges[predicate].push_back(Edge{subject,object});
            //compute syn 1
            inMap[predicate].insert({object,true});
            outMap[predicate].insert({subject,true});
            labelMap.insert({predicate,true});
            syn1[predicate].path++;
            ///////////////////////////////////
            adj[subject].emplace_back(Edge{predicate,object});
            ///////////////////////////////////
        }
    }
    L = labelMap.size();

    ///////////////////////////////
    ///////////////////////////////

    // some synapse computation
    syn2.resize(L);
    syn3.resize(L);
    syn4.resize(L);
    for (Label l = 0 ; l < L ; ++l){
        syn1[l].out = outMap[l].size();
        syn1[l].in  = inMap[l].size();
        //set size of syn2 and syn3 for later
        syn2[l].resize(L);
        syn3[l].resize(L);
        syn4[l].resize(L);
        /////*********************** Allocation *******************************
        sourceIndex[l].nbHeaders = syn1[l].out;
        sourceIndex[l].headers   = static_cast<Header*>(calloc(sourceIndex[l].nbHeaders, sizeof(Header)));
        sourceIndex[l].edges     = static_cast<Node*>(calloc(syn1[l].path, sizeof(Node)));
        sourceIndex[l].nbEdges   = syn1[l].path;

        targetIndex[l].nbHeaders = syn1[l].in;
        targetIndex[l].headers   = static_cast<Header*>(calloc(targetIndex[l].nbHeaders, sizeof(Header)));
        targetIndex[l].edges     = static_cast<Node*>(calloc(syn1[l].path, sizeof(Node)));
        targetIndex[l].nbEdges   = syn1[l].path;
        ///// -----------------------------------------------------------------
        randomAccessTableSource[l] = static_cast<RandomAccessTable>(calloc(V, sizeof(Node)));
        randomAccessTableTarget[l] = static_cast<RandomAccessTable>(calloc(V, sizeof(Node)));
        for (auto v = 0 ; v < V ; ++v){
            randomAccessTableSource[l][v] = NONE;
            randomAccessTableTarget[l][v] = NONE;
        }
        /////*********************** Construction ********************************
        insertAll(edges[l],l);
        /////*********************** END ********************************

        ///////////////////////////////


    }
    ////////////////////////////////////
    //*** allocation ***
    std::vector<std::vector<std::vector<std::vector<Node>>>> adjLabel2(L, std::vector<std::vector<std::vector<Node>>>());
    for (Label l1 = 0 ; l1 < L ; ++l1){
        adjLabel2[l1].resize(L);
        /////////////------------//////
        for (Label l2 = 0 ; l2 < L ; ++l2){
            adjLabel2[l1][l2].resize(V);
        }
    }
    //*** join ***
    for(uint32_t s = 0; s < V; s++) {
        for (auto mid : adj[s]) {
            Node join = mid.target;
            Label l1 = mid.source;
            // < < queries
            for (auto target : adj[join]) {
                Node t = target.target;
                Label l2 = target.source;
                adjLabel2[l1][l2][s].emplace_back(t);
            }
        }
    }
    //*** stats for index ****
    for (Label l1 = 0 ; l1 < L ; ++l1){
        for (Label l2 = 0 ; l2 < L ; ++l2){
            sourceIndex2[l1][l2].nbEdges = 0;
            sourceIndex2[l1][l2].nbHeaders = 0;
            for (auto targets = adjLabel2[l1][l2].begin() ; targets != adjLabel2[l1][l2].end() ; ++targets){
                if (!targets->empty()){
                    std::sort(targets->begin(), targets->end());
                    targets->erase( unique( targets->begin(), targets->end() ), targets->end());
                    sourceIndex2[l1][l2].nbEdges += targets->size();
                    sourceIndex2[l1][l2].nbHeaders++;
                }
            }
            sourceIndex2[l1][l2].headers   = static_cast<Header*>(calloc(sourceIndex2[l1][l2].nbHeaders, sizeof(Header)));
            sourceIndex2[l1][l2].edges     = static_cast<Node*>(calloc(sourceIndex2[l1][l2].nbEdges, sizeof(Node)));
        }
    }
    for (Label l1 = 0 ; l1 < L ; ++l1){
        for (Label l2 = 0 ; l2 < L ; ++l2){
            size_t header = 0;
            size_t nextSpace = 0;
            for (auto source = 0 ; source < V ; source++){
                if (!adjLabel2[l1][l2][source].empty()){
                    insertAll2(source,l1,l2,adjLabel2[l1][l2][source], sourceIndex2[l1]+l2,header,nextSpace);
                }
            }
        }
    }

    ////////////////////////////////////
    graphFile.close();
}

#define getSource(it) ((!reversed)?(*it).source:(*it).target)
#define getTarget(it) ((!reversed)?(*it).target:(*it).source)
void SimpleGraph::insertAll(std::vector<Edge> &edges, Label l, bool reversed, Comparator cmp, Index index) {
    std::sort(edges.begin(), edges.end(),cmp);
    auto it = edges.begin();
    size_t headerIndex = 0;
    size_t nextSpace = 0;
    Edge lastEdge = Edge{NONE,NONE};
    while (it != edges.end()){

        Node source = getSource(it);
        /////////////////////////////
         if (!reversed)randomAccessTableSource[l][source] = headerIndex;
         else          randomAccessTableTarget[l][source] = headerIndex;
        /////////////////////////////
        index[l].headers[headerIndex].source      = source;
        index[l].headers[headerIndex].indexEdges  = nextSpace;
        while(getSource(it) == source){ //for all edges that have source = (*it)
            //********* NO DUPLICATES ***********
            if((*it) == lastEdge){
                ++it;
                index[l].nbEdges--;
                continue;
            }
            lastEdge = *it;
            //********* REAL WORK ***********
            index[l].headers[headerIndex].nbTargets++;            //update size for each new target added
            index[l].edges[nextSpace] = getTarget(it);            //allocate target
            nextSpace++;                                                //next memory spot
            it++;//next edge

            //assert(nextSpace <=  (syn1[l].path));                     //check buffer overflow
        }
        headerIndex++;
        //assert(headerIndex <= index[l].nbHeaders);
    }
    if (syn1[l].path != index[l].nbEdges) syn1[l].path = index[l].nbEdges;
    void* ptr = realloc(index[l].edges, index[l].nbEdges *sizeof(Node));
    if (ptr == nullptr) throw "Memory error";
    index[l].edges = static_cast<Node *>(ptr);
    //assert(nextSpace  == syn1[l].path); //all memory has been used
    //assert(headerIndex == index[l].nbHeaders);           //all memory has been used
}
void SimpleGraph::insertAll(std::vector<Edge> &edges, Label l) {
    insertAll(edges,l,false,sourceComp,sourceIndex);
    insertAll(edges,l,true,targetComp, targetIndex);
}

//****************************************************
//********************** Access **********************
//****************************************************

#define getHeader(i) (index->headers[i])
Header* SimpleGraph::find(LabelIndex* index, Node source) {
    int lowerBound = 0;
    int upperBound = index->nbHeaders -1;
    int midPoint = -1;
    Header* res = nullptr;
    while(lowerBound <= upperBound) {
        midPoint = lowerBound + (upperBound-lowerBound)/2;
        if(getHeader(midPoint).source == source) {// data found
            res = index->headers + midPoint;
            break;
        } else {
            if(getHeader(midPoint).source < source) {lowerBound = midPoint + 1;}  // data is larger  => data is in upper half
            else                                               {upperBound = midPoint -1;}  // data is smaller => data is in lower half
        }
    }
    return res;
}

SimpleGraph::Iterator SimpleGraph::getEdges(Index index, QueryEdge queryEdge, bool needReverse) const{
    if (queryEdge.source == NONE){
        Header* start = index[queryEdge.label].headers;
        return Iterator(index + queryEdge.label,start,needReverse);
    }
    else {
        Header* start = find(index + queryEdge.label,queryEdge.source);
        if (start == nullptr) return Iterator();  //queryEdg.source is not in index
        Header* end   = start + 1;
        return Iterator(index + queryEdge.label,start,end,needReverse);
    }
}
SimpleGraph::Iterator SimpleGraph::getEdgesSource(QueryEdge queryEdge, bool needReverse) const {
    return getEdges(sourceIndex,queryEdge,needReverse);
}

SimpleGraph::Iterator SimpleGraph::getEdgesTarget(QueryEdge queryEdge, bool needReverse) const {
    return getEdges(targetIndex,reverse(queryEdge),needReverse);
}

SimpleGraph::Iterator SimpleGraph::getEdgesSource(QueryEdge queryEdge) const {
    return getEdgesSource(queryEdge, false);
}

SimpleGraph::Iterator SimpleGraph::getEdgesTarget(QueryEdge queryEdge) const {
    return getEdgesTarget(queryEdge,false);
}

//*************************************************************
//************************** Iterator *************************
//*************************************************************

SimpleGraph::Iterator::Iterator(LabelIndex *index, Header *start, Header* end, bool needReverse) {
    SimpleGraph::Iterator::index            = index;
    SimpleGraph::Iterator::source           = start;
    SimpleGraph::Iterator::target           = (index != nullptr)?index->edges + start->indexEdges: nullptr;
    SimpleGraph::Iterator::remainingTargets = (source != nullptr)?source->nbTargets:0;
    SimpleGraph::Iterator::end              = end;
    SimpleGraph::Iterator::needReverse      = needReverse;
    if (index != nullptr && index->nbHeaders == 0) done = true;
}
SimpleGraph::Iterator::Iterator(LabelIndex *index, Header *start, bool needReverse):
        Iterator(index,start, index->headers + index->nbHeaders, needReverse) {}
SimpleGraph::Iterator SimpleGraph::Iterator::operator++() {
    if (done) return *this;
        remainingTargets--;
        target++;
        if (remainingTargets > 0) {
            return *this;
        }
        else return skip();
}
Edge SimpleGraph::Iterator::operator*() {
    if (done) return Edge{NONE,NONE};
    return (!needReverse)?Edge{source->source, *target}: Edge{*target,source->source};
}

SimpleGraph::Iterator SimpleGraph::Iterator::skip() {
    ++source;
    if (source == end) {done = true; return *this;}
    target += remainingTargets;
    remainingTargets = source->nbTargets;
    return *this;
}

bool SimpleGraph::Iterator::hasNext() const {
    return !done;
}

SimpleGraph::Iterator::Iterator(): Iterator(nullptr, nullptr, nullptr,false) {
    done =true;
}


//*************************************************************
//********************** Graph Functions **********************
//*************************************************************

uint32_t SimpleGraph::getNoVertices() const {
    return V;
}

uint32_t SimpleGraph::getNoEdges() const {
    return 0;
}

uint32_t SimpleGraph::getNoDistinctEdges() const {
    return 0;
}

uint32_t SimpleGraph::getNoLabels() const {
    return L;
}

void SimpleGraph::addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) {

}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
SimpleGraph::IteratorReachable SimpleGraph::targetsReachable(Label label, Node source) {
    //return adjLabel[label][source];
    return IteratorReachable(sourceIndex+ label, source, randomAccessTableSource[label]);
}

SimpleGraph::IteratorReachable SimpleGraph::sourcesReachable(Label label, Node target) {
  //  return revAdjLabel[label][target];
    return IteratorReachable(targetIndex+ label, target, randomAccessTableTarget[label]);
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
SimpleGraph::IteratorReachable::IteratorReachable(LabelIndex *index, Node source, RandomAccessTable randomAccessTable) {
    if (randomAccessTable == nullptr) {
        auto h = find(index,source);
        if (h == nullptr) {remainingTargets = 0; return;}
        remainingTargets = h->nbTargets;
        targets = index->edges + h->indexEdges;
    }
    else {
        auto idx = randomAccessTable[source];
        if (idx == NONE) {remainingTargets = 0; return;}
        Header& h = index->headers[idx];
        remainingTargets = h.nbTargets;
        targets = index->edges + h.indexEdges;
    }
}

SimpleGraph::IteratorReachable SimpleGraph::IteratorReachable::operator++() {
    remainingTargets--;
    targets++;
    return (*this);
}

Node SimpleGraph::IteratorReachable::operator*() {
    return *targets;
}

bool SimpleGraph::IteratorReachable::hasNext() const {
    return remainingTargets > 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void SimpleGraph::insertAll2(Node source, Label l1,Label l2, std::vector<Node> &targets, LabelIndex* index, size_t& headerIndex, size_t& nextSpace) {
    ////////////////////////////
    //randomAccessTableSource2[l1][l2][source] = headerIndex;
    /////////////////////////////
    index->headers[headerIndex].source      = source;
    index->headers[headerIndex].indexEdges  = nextSpace;
    index->headers[headerIndex].nbTargets   = targets.size();
    for (auto t : targets){
        index->edges[nextSpace] = t;
        nextSpace++;
    }
    headerIndex++;
}

IteratorReachable SimpleGraph::targetsReachable2(Label l1, Label l2, Node source) {
    return IteratorReachable(sourceIndex2[l1]+l2, source, nullptr);
}

SimpleGraph::Iterator SimpleGraph::getEdgesTarget2(Label l1, Label l2) const {
        return Iterator(sourceIndex2[l1]+l2,sourceIndex2[l1][l2].headers,false);
}

