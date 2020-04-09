//
// Created by K.Mankevic on 26.03.2020.
//

#ifndef QUICKSILVER_SUBTREE_H
#define QUICKSILVER_SUBTREE_H

#include "iostream"
#include <vector>

struct Subtree {

    // this subtree is always of depth 1.
    // we do not store the root, but only its children as vector of strings:
    std::vector<std::string> subtree;
    // store the start index of the subtree w.r.t complete children vector
    uint32_t s_index;
    // store the end index of the subtree w.r.t complete children vector
    uint32_t e_index;

    // Do something in case of collisions.
    bool operator == (const Subtree& p) const
    {
        return (s_index == p.s_index && e_index == p.e_index);
    }
};



class SubtreeHash {
public:
    // Use concatenated string as a param to standard string hash function
    // concatenated string holds all children of this subtree, together with start and end indices of this subtree.
    size_t operator()(const Subtree& subtree) const
    {
        std::string str_for_hash;
        //append start and end indices to the string (which will be hashed) as well:
        str_for_hash.append(std::to_string(subtree.s_index));
        str_for_hash.append("/");
        str_for_hash.append(std::to_string(subtree.e_index));
        // str_children holds concatenated children
        std::hash<std::string> hash;
        return hash(str_for_hash);
    }
};


#endif //QUICKSILVER_SUBTREE_H
