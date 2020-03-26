#include "SimpleGraph.h"

SimpleGraph::SimpleGraph(uint32_t n)   {
    setNoVertices(n);
}

uint32_t SimpleGraph::getNoVertices() const {
    return V;
}

void SimpleGraph::setNoVertices(uint32_t n) {
    V = n;
}

uint32_t SimpleGraph::getNoLabels() const {
    return L;
}

void SimpleGraph::setNoLabels(uint32_t noLabels) {
    L = noLabels;
}

void SimpleGraph::addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) {
   insert(Edge{from, to},edgeLabel);
}

bool SimpleGraph::edgeExists(uint32_t from, uint32_t to, uint32_t edgeLabel) {
    auto res = getEdgesSource(QueryEdge{from, edgeLabel, to});
    return res.first != res.second;
}

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
        mapSource.resize(L);
        mapTarget.resize(L);
    } else {
        throw std::runtime_error(std::string("Invalid graph header!"));
    }
    std::vector<std::vector<Edge>> edges(L, std::vector<Edge>());

    //std::vector<Edge> edges;  //holds the edges
    // parse edge data
    while(std::getline(graphFile, line)) {

        if(std::regex_search(line, matches, edgePat)) {
            uint32_t subject = (uint32_t) std::stoul(matches[1]);
            uint32_t predicate = (uint32_t) std::stoul(matches[2]);
            uint32_t object = (uint32_t) std::stoul(matches[3]);

            edges[predicate].push_back(Edge{subject,object});
        }
    }
    graphFile.close();

    /*Construct index*/
    for (int l = 0 ; l <L ; ++l){
        insertAll(edges[l],l);
    }
}
/**************************************  METHODS FROM EDGE INDEX ********************************************/

Edge toPrefix(QueryEdge e) {
    return Edge{(e.source==NONE)?0:e.source, (e.target==NONE)?0:e.target};
}

Edge nextIncrementalEdge(QueryEdge &e) {
    Edge nextEdge = toPrefix(e);
    if (e.source ==NONE) nextEdge.source=NONE;
    else                 nextEdge.source++;
    return nextEdge;
}

IndexResult SimpleGraph::getEdgesSource(QueryEdge edgePrefix) const {
    //assert(edgePrefix.label != NONE && !(edgePrefix.source == NONE && edgePrefix.target != NONE));
    return getEdges(edgePrefix, mapSource[edgePrefix.label]);
}

IndexResult SimpleGraph::getEdgesTarget(QueryEdge edgePrefix) const {
    //assert(edgePrefix.label != NONE && !(edgePrefix.source != NONE && edgePrefix.target == NONE));
    return getEdges(reverse(edgePrefix), mapTarget[edgePrefix.label]);
}

IndexResult SimpleGraph::getEdges(QueryEdge edgePrefix, const std::map<Edge,Edge> &map) const {
    return {map.lower_bound(toPrefix(edgePrefix)),map.upper_bound(nextIncrementalEdge(edgePrefix))};
}

void SimpleGraph::insert(Edge e, Node l) {
    mapSource[l].insert({e,e});
    mapTarget[l].insert({reverse(e),reverse(e)});
}

void SimpleGraph::insertAll(std::vector<QueryEdge> edges) {
    std::sort(edges.begin(), edges.end(),labelSourceComp);
    for (auto e : edges) {
       // assert(e.target != NONE && e.label != NONE && e.target != NONE);
        Edge o = Edge{e.source,e.target};
        mapSource[e.label].insert(mapSource[e.label].cend(), {o,o});
    }
    std::sort(edges.begin(), edges.end(),labelTargetComp);
    for (auto e : edges) {
        Edge o = Edge{e.source,e.target};
        mapTarget[e.label].insert(mapTarget[e.label].cend(), {reverse(o),reverse(o)});
    }
}
void SimpleGraph::insertAll(std::vector<Edge> edges, Label l) {
    std::sort(edges.begin(),edges.end(),sourceComp);
    for (auto e : edges) {
        mapSource[l].insert(mapSource[l].cend(), {e,e});
    }
    std::sort(edges.begin(), edges.end(),targetComp);
    for (auto e : edges) {
        mapTarget[l].insert(mapTarget[l].cend(), {reverse(e),reverse(e)});
    }
}

uint32_t SimpleGraph::getNoEdges() const {
    return 0;
}

uint32_t SimpleGraph::getNoDistinctEdges() const {
    return 0;
}