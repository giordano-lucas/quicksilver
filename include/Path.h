//
// Created by Lucas Giordano on 07/03/2020.
//

#ifndef QUICKSILVER_PATH_H
#define QUICKSILVER_PATH_H
#include <iostream>
#include <vector>


typedef uint32_t Node;
typedef uint32_t Label;
class Path {
private:
    size_t id;
    std::vector<Node> nodes;
    bool prefix;
public:
    /*  FUNCTIONS TO IMPLEMENT */
    Path(const bool prefix, const std::vector<Label> &labels, uint32_t noOfLabels, const std::vector<Node> &nodes);
    bool operator<(const Path& other) const;

    /*  General index constructors valid for any K */
    Path(const std::vector<Label> &labels, uint32_t noOfLabels, const std::vector<Node> &nodes): Path(false, labels, noOfLabels, nodes) {}
    Path(const bool prefix,size_t id, const std::vector<Node> &nodes) : Path(prefix, std::vector<uint32_t>((nodes.size()==0)?0:nodes.size()-1,id), 0, nodes){}
    static Path PrefixPath(const std::vector<Label> &labels, uint32_t noOfLabels, const std::vector<Node> &nodes) { return Path(true, labels, noOfLabels, nodes);};

    /*  Specialised index function valid for K  = 1*/
    Path(const bool prefix, Node a,Label l, Node b) : Path(prefix,std::vector<Label>{l}, 0, std::vector<Node>{a,b}){}
    Path(Node a,Label l, Node b) : Path(false, a,l, b){}
    static Path PrefixPath(Label l, const std::vector<Node> &nodes) {return PrefixPath(std::vector<Label>{l}, 0,  nodes);};

    /*  Access functions */
    uint64_t getId()             const {return id;};
    std::vector<Node> getNodes() const {return nodes;};
    bool isPrefix()              const {return prefix;};
    size_t length()              const {return nodes.size();};
};

std::ostream& operator<<(std::ostream &strm, const Path &path);
#endif //QUICKSILVER_PATH_H

