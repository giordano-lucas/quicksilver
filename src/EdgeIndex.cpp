//
// Created by Lucas Giordano on 11/03/2020.
//

#include "EdgeIndex.h"


Edge EdgeIndex::nextIncrementalEdge(Edge &e, bool reversed) const {
    if (e.label == NONE) return Edge{e.source, e.label+1, e.target};
    //if (reversed) return Edge{e.source+1, e.label,e.target};
    else          return Edge{e.source, e.label,e.target+1};
}

IndexResult EdgeIndex::getEdges(Edge edgePrefix) const {
        if (edgePrefix.target == NONE)  return {mapSource.lower_bound(edgePrefix),mapSource.upper_bound(nextIncrementalEdge(edgePrefix,false))};
        else {
            Edge reversedEdgePrefix = reverse(edgePrefix);
            return {mapTarget.lower_bound(reversedEdgePrefix),mapTarget.upper_bound(nextIncrementalEdge(reversedEdgePrefix,false))};
        }
}

IndexResult EdgeIndex::allEdges(bool reversed) const {
    IndexResult res = {mapSource.begin(), mapSource.end()};
    if (reversed) res = {mapTarget.begin(), mapTarget.end()};
    return res;
}

void EdgeIndex::insert(Edge e) {
    assert(e.target != NONE && e.label != NONE && e.target != NONE);
    mapSource.insert({e,e});
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
