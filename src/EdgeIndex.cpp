//
// Created by Lucas Giordano on 11/03/2020.
//

#include "EdgeIndex.h"

Edge EdgeIndex::toPrefix(Edge e) const {
    return Edge{(e.source==NONE)?0:e.source, e.label, (e.target==NONE)?0:e.target};
}

Edge EdgeIndex::nextIncrementalEdge(Edge &e) const {
    Edge nextEdge = toPrefix(e);
    if (e.source ==NONE) nextEdge.label++;
    else                 nextEdge.source++;
    return nextEdge;
}

IndexResult EdgeIndex::getEdgesSource(Edge edgePrefix) const {
    assert(edgePrefix.label != NONE && !(edgePrefix.source == NONE && edgePrefix.target != NONE));
    return getEdges(edgePrefix, mapSource);
}

IndexResult EdgeIndex::getEdgesTarget(Edge edgePrefix) const {
    assert(edgePrefix.label != NONE && !(edgePrefix.source != NONE && edgePrefix.target == NONE));
    return getEdges(reverse(edgePrefix), mapTarget);
}

IndexResult EdgeIndex::getEdges(Edge edgePrefix, const Map &map) const {
    return {map.lower_bound(toPrefix(edgePrefix)),map.upper_bound(nextIncrementalEdge(edgePrefix))};
}

IndexResult EdgeIndex::allEdges(bool reversed) const {
    IndexResult res = {mapSource.begin(), mapSource.end()};
    if (reversed) res = {mapTarget.begin(), mapTarget.end()};
    return res;
}

void EdgeIndex::insert(Edge e) {
    assert(e.target != NONE && e.label != NONE && e.target != NONE);
    mapSource.insert({e,e});
    mapTarget.insert({reverse(e),reverse(e)});
}


void EdgeIndex::insertAll(std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(),labelSourceComp);
    for (auto e : edges) {
        assert(e.target != NONE && e.label != NONE && e.target != NONE);
        mapSource.insert(mapSource.cend(), {e,e});
    }
    std::sort(edges.begin(), edges.end(),labelTargetComp);
    for (auto e : edges) {
        mapTarget.insert(mapTarget.cend(), {reverse(e),reverse(e)});
    }
}

void EdgeIndex::buildFromFile(const std::string &fileName) {
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
    } else {
        throw std::runtime_error(std::string("Invalid graph header!"));
    }

    std::vector<Edge> edges;  //holds the edges
    // parse edge data
    while(std::getline(graphFile, line)) {

        if(std::regex_search(line, matches, edgePat)) {
            uint32_t subject = (uint32_t) std::stoul(matches[1]);
            uint32_t predicate = (uint32_t) std::stoul(matches[2]);
            uint32_t object = (uint32_t) std::stoul(matches[3]);

            edges.push_back(Edge{subject,predicate,object});
        }
    }
    graphFile.close();

    /*Construct index*/
    auto start = std::chrono::steady_clock::now();
    std::sort(edges.begin(), edges.end()); //paths
    insertAll(edges);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time to construct the index : " << (std::chrono::duration<double, std::milli>(end - start).count())/1000 << " ms" << std::endl;
}

std::ostream& operator<<(std::ostream &strm, const EdgeIndex &index){
    strm << "---------- IndexEdge ---------- \n";
    strm << "--- Source --- \n";
    for (IndexIterator it = index.allEdges(false).first ; it != index.allEdges(false).second ; ++it){
        strm << it->first ;
    }
    strm << "--- Target --- \n";
    for (IndexIterator it = index.allEdges(true).first ; it != index.allEdges(true).second ; ++it){
        strm << it->first ;
    }
    strm << "\n";
    return strm;
}
