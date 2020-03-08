//
// Created by Lucas Giordano on 07/03/2020.
//

#ifndef QUICKSILVER_INDEXPATH_H
#define QUICKSILVER_INDEXPATH_H
#include<iterator> // for iterators
#include <map>
#include "Path.h"
#include "Graph.h"

typedef std::map<Path,Path> Map;
typedef Map::const_iterator  IndexIterator;
typedef std::pair<IndexIterator,IndexIterator> IndexResult;
class IndexPath {
private:
    Map index;
    Path nextIncrementalPath(Path path) const;
public:
    /*Constructor*/
    IndexPath() : index(){};
    /*Access methods*/
    IndexResult getPaths(Path pathPrefix) const;
    IndexResult allPaths() const;
    /*Insertion methods*/
    void insert(Path path);
    void insertSorted(Path path);
    void insertAll(std::vector<Path> paths);
    void insertSortedAll(std::vector<Path> paths);
    /*Scale up to a higher k*/
    void buildK2(uint32_t noOfLabels);
    void buildK2(std::vector<Path>& edges,uint32_t noOfLabels);
    void buildK2MergeJoin(std::vector<Path>& edges, uint32_t noOfLabels);
};
std::ostream& operator<<(std::ostream &strm, const IndexPath &path);
#endif //QUICKSILVER_INDEXPATH_H
