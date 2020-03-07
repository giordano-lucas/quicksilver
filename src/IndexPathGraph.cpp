//
// Created by Lucas Giordano on 07/03/2020.
//

#include "IndexPathGraph.h"


uint32_t IndexPathGraph::getNoVertices() const {
    return V;
}

uint32_t IndexPathGraph::getNoEdges() const {
    IndexResult res = index.allPaths();
    uint32_t count = 0;
    for (IndexIterator it = res.first ; it != res.second && it->first.getId() < L; ++it) ++count;
    return count;
}

uint32_t IndexPathGraph::getNoDistinctEdges() const {
    return getNoEdges();
}

uint32_t IndexPathGraph::getNoLabels() const {
    return L;
}

void IndexPathGraph::addEdge(uint32_t from, uint32_t to, uint32_t edgeLabel) {
    /*TO DO LATER*/
    /*NEED TO ALSO  UPDATE K=2 index*/
}

void IndexPathGraph::readFromContiguousFile(const std::string &fileName) {
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

    std::vector<Path> edges;  //holds the edges
    // parse edge data
    while(std::getline(graphFile, line)) {

        if(std::regex_search(line, matches, edgePat)) {
            uint32_t subject = (uint32_t) std::stoul(matches[1]);
            uint32_t predicate = (uint32_t) std::stoul(matches[2]);
            uint32_t object = (uint32_t) std::stoul(matches[3]);


            edges.push_back(Path(subject,predicate,object));
        }
    }
    graphFile.close();

    /*Construct index*/
    std::cout << "BEFORE SORTING \n";
    std::sort(edges.begin(), edges.end()); //paths
    std::cout << "AFTER SORTING \n ";
    index.insertSortedAll(edges);
    std::cout << "BEFORE BUILDING K=2 \n";
    index.buildK2(edges,L);

}
