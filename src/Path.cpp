//
// Created by Lucas Giordano on 07/03/2020.
//

#include "Path.h"
#include <functional>
#include <algorithm>

/*
 * Main constructor
 * */
Path::Path(const bool isPrefix, const std::vector<Label> &labels, uint32_t noOfLabels, const std::vector<Node> &nodes) : nodes(nodes) {
    if (!isPrefix && (nodes.size() - labels.size() != 1)) fprintf(stderr, "Path creation : nodes size and label size do not match");

    id = 0; // id = (((l1+1)*L + l2+1)*L + ... )*L + ln   => gives unique id for all combinaison of labels
    for (size_t i = 0 ; i < labels.size() ; ++i){
        if (i != 0) ++id;
        id = (id*noOfLabels) + labels[i];
    }
    for(auto l : labels){

    }
    prefix = isPrefix;
}
/*
 * Overloading of < operator => define a ordering between paths
 * */
bool Path::operator<(const Path& o) const {
    if (getId() != o.getId()) return getId() < o.getId();
    if ((isPrefix() && o.isPrefix()) || (!isPrefix() && !o.isPrefix())){ //compare prefix to prefix or full path to full path -> compare first the number of nodes
        if (length() != o.length()) return length() < o.length();
    }
    for (int i = 0; i < std::min(length(),o.length()); i++) {
        if (getNodes()[i] != o.getNodes()[i]) {
            return getNodes()[i] < o.getNodes()[i];
        }
    }
    if (length() != o.length()) return length() < o.length(); //only for prefix to full path comparison
    return false; //equal case
}

/*
 * Overloading of << operator (toString)
 * */
std::ostream& operator<<(std::ostream &strm, const Path &path) {
    strm << "Path(id=" << path.getId() << ") = ";
    bool hasPrevious = false;
    for (auto n : path.getNodes()){
        if (hasPrevious) {strm <<" -> ";}
        strm << "(" << n <<")";
        hasPrevious = true;
    }
    strm << "\n";
    return strm;
}



