//
// Created by Lucas Giordano on 27/03/2020.
//

#include "SmartGraph.h"


//****************************************************
//******************* Destructor *********************
//****************************************************

SmartGraph::~SmartGraph()  {
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

void SmartGraph::readFromContiguousFile(const std::string &fileName) {
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
        /////***************************** END ********************************
    } else {
        throw std::runtime_error(std::string("Invalid graph header!"));
    }
    std::vector<std::vector<Edge>> edges(L, std::vector<Edge>());
    //set syn1
    syn1.resize(L);
    std::vector<std::unordered_map<Node,bool>> inMap(L, std::unordered_map<Node,bool>());
    std::vector<std::unordered_map<Node,bool>> outMap(L, std::unordered_map<Node,bool>());


    while(std::getline(graphFile, line)) {

        if(std::regex_search(line, matches, edgePat)) {
            uint32_t subject = (uint32_t) std::stoul(matches[1]);
            uint32_t predicate = (uint32_t) std::stoul(matches[2]);
            uint32_t object = (uint32_t) std::stoul(matches[3]);
            edges[predicate].push_back(Edge{subject,object});
            //compute syn 1
            inMap[predicate].insert({object,true});
            outMap[predicate].insert({subject,true});
            syn1[predicate].path++;

        }
    }
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
        /////*********************** Construction ********************************
        insertAll(edges[l],l);
        /////*********************** END ********************************
    }
    graphFile.close();
}

#define getSource(it) ((!reversed)?(*it).source:(*it).target)
#define getTarget(it) ((!reversed)?(*it).target:(*it).source)
void SmartGraph::insertAll(std::vector<Edge> &edges, Label l, bool reversed, Comparator cmp, Index index) {
    std::sort(edges.begin(), edges.end(),cmp);
    auto it = edges.begin();
    size_t headerIndex = 0;
    size_t nextSpace = 0;
    Edge lastEdge = Edge{NONE,NONE};
    while (it != edges.end()){
        //********* NO DUPLICATES ***********
       /* if ((*it) == lastEdge){
            ++it;
            syn1[l].path--;
        }
        lastEdge = *it;*/
        //********* REAL WORK ***********
        Node source = getSource(it);
        index[l].headers[headerIndex].source      = source;
        index[l].headers[headerIndex].indexEdges  = nextSpace;
        while(getSource(it) == source){ //for all edges that have source = (*it)
            index[l].headers[headerIndex].nbTargets++;            //update size for each new target added
            index[l].edges[nextSpace] = getTarget(it);            //allocate target
            nextSpace++;                                                //next memory spot
            it++;                                                       //next edge
            assert(nextSpace <=  (syn1[l].path));                        //check buffer overflow
        }
        headerIndex++;
        assert(headerIndex <= index[l].nbHeaders);
    }
    assert(nextSpace  == syn1[l].path); //all memory has been used
    assert(headerIndex == index[l].nbHeaders);           //all memory has been used
}
void SmartGraph::insertAll(std::vector<Edge> &edges, Label l) {
    insertAll(edges,l,false,sourceComp,sourceIndex);
    insertAll(edges,l,true,targetComp, targetIndex);
}

//****************************************************
//********************** Access **********************
//****************************************************

#define getHeader(i) (index[queryEdge.label].headers[i])
Header* SmartGraph::find(Index index,QueryEdge queryEdge) const {
    int lowerBound = 0;
    int upperBound = index[queryEdge.label].nbHeaders -1;
    int midPoint = -1;
    Header* res = nullptr;
    while(lowerBound <= upperBound) {
        midPoint = lowerBound + (upperBound-lowerBound)/2;
        if(getHeader(midPoint).source == queryEdge.source) {// data found
            res = index[queryEdge.label].headers + midPoint;
            break;
        } else {
            if(getHeader(midPoint).source < queryEdge.source) {lowerBound = midPoint + 1;}  // data is larger  => data is in upper half
            else                                               {upperBound = midPoint -1;}  // data is smaller => data is in lower half
        }
    }
    assert(res != nullptr);
    return res;
}

SmartGraph::Iterator SmartGraph::getEdges(Index index, QueryEdge queryEdge, bool needReverse) const{
    if (queryEdge.source == NONE){
        Header* start = index[queryEdge.label].headers;
        return Iterator(index + queryEdge.label,start,needReverse);
    }
    else {
        Header* start = find(index,queryEdge);
        Header* end   = start + 1;
        return Iterator(index + queryEdge.label,start,end,needReverse);
    }
}
SmartGraph::Iterator SmartGraph::getEdgesSource(QueryEdge queryEdge, bool needReverse) const {
    return getEdges(sourceIndex,queryEdge,needReverse);
}

SmartGraph::Iterator SmartGraph::getEdgesTarget(QueryEdge queryEdge, bool needReverse) const {
    return getEdges(targetIndex,reverse(queryEdge),needReverse);
}

SmartGraph::Iterator SmartGraph::getEdgesSource(QueryEdge queryEdge) const {
    return getEdgesSource(queryEdge, false);
}

SmartGraph::Iterator SmartGraph::getEdgesTarget(QueryEdge queryEdge) const {
    return getEdgesTarget(queryEdge,false);
}

//*************************************************************
//************************** Iterator *************************
//*************************************************************


SmartGraph::Iterator::Iterator(LabelIndex *index, Header *start, Header* end, bool needReverse) {
    SmartGraph::Iterator::index            = index;
    SmartGraph::Iterator::source           = start;
    SmartGraph::Iterator::target           = index->edges + start->indexEdges;
    SmartGraph::Iterator::remainingTargets = source->nbTargets;
    SmartGraph::Iterator::end              = end;
    SmartGraph::Iterator::needReverse      = needReverse;
}
SmartGraph::Iterator::Iterator(LabelIndex *index, Header *start, bool needReverse):
        Iterator(index,start, index->headers + index->nbHeaders, needReverse) {}
SmartGraph::Iterator SmartGraph::Iterator::operator++() {
    if (done) return *this;
    if (!needSorted){          //regular iterator
        remainingTargets--;
        target++;
        if (remainingTargets > 0) {
            return *this;
        }
        else return skip();
    }
    else {//sorted iterator
        ++itSorted;
        if (itSorted >= (sortedArray + index->nbEdges)) {
            done = true;
            free(allocatedArray);
        }
        return *this;
    }

}
Edge SmartGraph::Iterator::operator*() {
    if (done) return Edge{NONE,NONE};
    if (!needSorted) return (!needReverse)?Edge{source->source, *target}: Edge{*target,source->source};
    else             return *itSorted;
}

SmartGraph::Iterator SmartGraph::Iterator::skip() {
    uint32_t nbHeaders = index->nbHeaders;
    ++source;
    if (source == end) {done = true; return *this;}
    target += remainingTargets;
    remainingTargets = source->nbTargets;
    return *this;
}

bool SmartGraph::Iterator::hasNext() const {
    return !done;
}

SmartGraph::Iterator SmartGraph::Iterator::sort(Comparator cmp) {
    if (needSorted) return *this; //already sorted
    allocatedArray = static_cast<Edge*>(calloc(index->nbEdges, sizeof(Edge)));
    sortedArray = allocatedArray;
    itSorted = sortedArray;
    while (hasNext()){
        (*itSorted) = operator*();
        itSorted++;
        operator++();
    }
    itSorted = sortedArray;
    std::sort(sortedArray,sortedArray + index->nbEdges, cmp);
    done = false;
    needSorted = true;
    source = nullptr;
    end = nullptr;
    target = nullptr;
    return *this;
}


//*************************************************************
//********************** Graph Functions **********************
//*************************************************************

uint32_t SmartGraph::getNoVertices() const {
    return V;
}

uint32_t SmartGraph::getNoEdges() const {
    return 0;
}

uint32_t SmartGraph::getNoDistinctEdges() const {
    return 0;
}

uint32_t SmartGraph::getNoLabels() const {
    return L;
}

void SmartGraph::addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) {

}


