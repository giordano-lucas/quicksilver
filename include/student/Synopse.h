//
// Created by Lucas Giordano on 27/03/2020.
//

#ifndef QUICKSILVER_SYNOPSE_H
#define QUICKSILVER_SYNOPSE_H
#include <iostream>
/**
 * Synopse that stores information related to path of length 1 in the graph, for each label
 * */
struct Syn1{
    uint32_t out    =0;  // * number of nodes in G which have outgoing edge labeled with l
    uint32_t in     =0;  // * number of nodes in G which have incoming edge labeled with l
    uint32_t path   =0;  // * number of paths in G labeled with l
    //uint32_t pairs  =0;  //number of distinct node pairs connected with paths labeled with l.
};
/**
 * Synopse that stores information related to path of length 2 in the graph, for each ordered
 * pair of labels l1 and l2
 * */
struct Syn2{
    //uint32_t out    =0;  //number of nodes in G which have outgoing path labeled with l1/l2
    uint32_t middle =0;  // * number of nodes in G which have incoming edge labeled l1 and outgoing edge labeled l2,
    uint32_t in     =0;  // * number of nodes in G which have incoming path labeled with l1/l2
    //uint32_t path   =0;  //number of paths in G labeled with l1/l2
    // uint32_t pairs  =0;  //number of distinct node pairs connected with paths labeled with l1/l2.
    //uint32_t one    =0;  //number of paths labeled l1 from nodes in out to nodes in middle, and
    uint32_t two    =0;  // * number of paths labeled l2 from nodes in middle to nodes in in.
};
/**
 * Synopse that stores information related to nodes that have both incoming/outgoing edges l1 and l2,
 * for each ordered pair of labels l1 and l2
 * */
struct Syn3{
    uint32_t middleIn  =0;  // * number of nodes in G which have incoming edge labeled l1 and incoming edge labeled l2,
    uint32_t in        =0;  // * number of nodes in G which have incoming path labeled with l1/~l2
    uint32_t twoIn     =0;  //* number of path from nodes in middleIn to nodes in syn3.in
};

struct Syn4{
    uint32_t middleOut = 0; // * number of nodes in G which have outgoing edge labeled l1 and outgoing edge labeled l2,
    uint32_t out       =0; // * number of nodes in G which have incoming path labeled with ~l1/l2
    uint32_t twoOut    =0; //* number of path from nodes in middleOut to nodes in syn4.out
};
#endif //QUICKSILVER_SYNOPSE_H
