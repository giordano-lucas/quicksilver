//
// Created by Lucas Giordano on 07/03/2020.
//

#include "IndexPath.h"

/*
 * Computes the smallest path that does not satisfy the (Prefix)path
 *
 * Ex  : if the inital query wast q = {Path(id=7) = (0)->(5)->(*)->(*)} in a K-path index where K = 4.
 *       we need to return all the path is the index having a path > q and < {Path(id=7) = (0)->(6)->(*)->(*)}
 *       This could return the following paths for example :
 *                                          - {Path(id=7) = (0)->(5)->(2)->(0)}
 *                                          - {Path(id=7) = (0)->(5)->(6)->(9)}
 * */
Path IndexPath::nextIncrementalPath(Path path) const{
    std::vector<Node> newNodes(path.getNodes()); //deep copy of nodes
    if (newNodes.size() == 0) {                  //the prefix contains no nodes -> the results is the path with a higher id
        return Path(path.isPrefix(),path.getId()+1,  newNodes);
    }
    else {                                      //The path constains at least one node, we want all nodes up to the last node id +1
        Node last = newNodes.back();
        newNodes.pop_back();
        newNodes.push_back(last+1);
        return Path(path.isPrefix(),path.getId(),  newNodes);
    }
}
/*
 * Returns the paths greater than pathPrefix and smaller than nextIncrementalPath(pathPrefix) in the index
 * */
IndexResult IndexPath::getPaths(Path pathPrefix) const {
    return {index.lower_bound(pathPrefix),index.upper_bound(nextIncrementalPath(pathPrefix))};
}
/*
 * Returns all path in the index
 * */
IndexResult IndexPath::allPaths() const {
    return {index.cbegin(), index.cend()};
};
/*
 * Inserts a path into the index : complexity O(log(N))
 * */
void IndexPath::insert(Path path) {
    assert(!path.isPrefix()); //allow only full path in index
    index.insert({path,path});
}
/*
 * Inserts a path that is larger than all paths currently stored into the index into the index : complexity O(1)
 * */
void IndexPath::insertSorted(Path path) {
    assert(!path.isPrefix());
    index.insert(index.cend(), {path,path});
}

/*
 * Inserts the entire collection of paths into the index  : complexity O((N+M)(log(N+M))) where M = paths.size()
 * */
void IndexPath::insertAll(std::vector<Path> paths) {
    for (auto path : paths) insert(path);
}
/*
 * Inserts the entire collection of paths into the index  : complexity O(M) where M = paths.size()
 * */
void IndexPath::insertSortedAll(std::vector<Path> paths) {
    for (auto path : paths) insertSorted(path);
}
/*
 * Build the K=2 path index from the K=1 index
 * */
void IndexPath::buildK2(uint32_t noOfLabels) {
    /* For each label => add the iterator for which the first path has label 1*/
    Label l = 0;
    std::vector<IndexIterator> partitionLabel;
    partitionLabel.push_back(IndexIterator(index.begin()));
    for (IndexIterator it = index.begin(); it != index.end() ; ++it){
        if (it->first.getId() > l) {
            partitionLabel.push_back(IndexIterator(it));
            l++;
        }
    }
    /* Construct K2*/
    std::vector<Path> newPaths; //stores the length 2 path to insert

    /*
     * Joins the path of length 1 such that the newPaths is sorted by construction
     *
     * Algo : perform joins by block in the following way :
     *          - [0][0] : each l1=0 and l2=0 are joigned
     *          - [0][1] : each l1=0 and l1=0 are joigned
     *          - [0]...
     *          - [0][L-1] : each l1=0 and l1=L-1 are joigned
     *
     *          - [1][0]
     *          - [1]...
     *          - [1][L-1]
     *
     *          - ......
     *
     *          - [L-1][0]
     *          - [L-1]...
     *          - [L-1][L-1]
     *
     *  This 4-for-loop is thus in fact a structered 2-for-loop  and has thus the same complexity
     *  This produces indeed a sorted list since the ids for the l2 path are computed with the following formula : (l1+1)*L + l2
     * */
    for (size_t idA = 0 ; idA < noOfLabels ; ++idA){
        std::cout << "LA = " << idA << "\n";
        for (size_t idB = 0 ; idB < noOfLabels ; ++idB){
            std::cout << "  LB = " << idB << "\n";
            for (IndexIterator itA = IndexIterator(partitionLabel[idA]); itA != index.end() && itA->first.getId() <= idA ; ++itA) {
                std::vector<Node> nodesA = itA->first.getNodes();
                for (IndexIterator itB = IndexIterator(partitionLabel[idB]); itB != index.end() && itB->first.getId() <= idB; ++itB) {
                    std::vector<Node> nodesB = itB->first.getNodes();
                    if (nodesA.back() == nodesB.front()) { //matching path
                        std::vector<Node> nodes(nodesA);
                        nodes.pop_back();
                        nodes.insert(nodes.end(), nodesB.begin(), nodesB.end());
                        newPaths.push_back(
                                Path(false, (itA->first.getId() + 1) * noOfLabels + itB->first.getId(), nodes));
                    }
                }
            }
        }
    }
    insertSortedAll(newPaths); //we know that the list is storted by construction
}
/*
 * Build the K=2 path index from the K=1 index
 * */
void IndexPath::buildK2(std::vector<Path>& edges, uint32_t noOfLabels) {
    /* For each label => add the iterator for which the first path has label 1*/
    Label l = 0;
    std::vector<uint32_t> partitionLabel;
    partitionLabel.push_back(0);
    for (uint32_t i = 0; i < edges.size() ; ++i){
        if (edges[i].getId() > l) {
            partitionLabel.push_back(i);
            l++;
        }
    }
    /* Construct K2*/
    std::vector<Path> newPaths; //stores the length 2 path to insert

    /*
     * Joins the path of length 1 such that the newPaths is sorted by construction
     *
     * Algo : perform joins by block in the following way :
     *          - [0][0] : each l1=0 and l2=0 are joigned
     *          - [0][1] : each l1=0 and l1=0 are joigned
     *          - [0]...
     *          - [0][L-1] : each l1=0 and l1=L-1 are joigned
     *
     *          - [1][0]
     *          - [1]...
     *          - [1][L-1]
     *
     *          - ......
     *
     *          - [L-1][0]
     *          - [L-1]...
     *          - [L-1][L-1]
     *
     *  This 4-for-loop is thus in fact a structered 2-for-loop  and has thus the same complexity
     *  This produces indeed a sorted list since the ids for the l2 path are computed with the following formula : (l1+1)*L + l2
     * */
    for (size_t idA = 0 ; idA < noOfLabels ; ++idA){
        std::cout << "LA = " << idA << "\n";
        for (size_t idB = 0 ; idB < noOfLabels ; ++idB){
            std::cout << "  LB = " << idB << "\n";
            for (uint32_t  i = partitionLabel[idA]; i < edges.size() && edges[i].getId() <= idA ; ++i) {
                std::vector<Node> nodesA = edges[i].getNodes();
                for (uint32_t  j = partitionLabel[idB]; j < edges.size() && edges[j].getId() <= idB ; ++j) {
                    std::vector<Node> nodesB = edges[j].getNodes();
                    if (nodesA.back() == nodesB.front()) { //matching path
                        std::vector<Node> nodes(nodesA);
                        nodes.pop_back();
                        nodes.insert(nodes.end(), nodesB.begin(), nodesB.end());
                        newPaths.push_back(
                                Path(false, (edges[i].getId() + 1) * noOfLabels + edges[j].getId(), nodes));
                    }
                }
            }
        }
    }
    std::cout << "GENERATED K=2 => " << newPaths.size() << "\n";
    insertSortedAll(newPaths); //we know that the list is storted by construction
}

/*
 * Overloading of << operator (toString)
 * */
std::ostream& operator<<(std::ostream &strm, const IndexPath &index){
    strm << "---------- IndexPath ---------- \n";
    for (IndexIterator it = index.allPaths().first ; it != index.allPaths().second ; ++it){
        strm << it->first ;
    }
    strm << "\n";
    return strm;
}