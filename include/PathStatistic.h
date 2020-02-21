
#ifndef QUICKSILVER_PATHSTATISTIC_H
#define QUICKSILVER_PATHSTATISTIC_H
#include <memory>
#include "SimpleGraph.h"
#include "Estimator.h"
#include <cstdint>

struct cardPathStat{
    uint32_t l;
    cardStat stat; 
};
struct Syn1{
    uint32_t out    =0;  // * number of nodes in G which have outgoing edge labeled with l
    uint32_t in     =0;  // * number of nodes in G which have incoming edge labeled with l
    uint32_t path   =0;  // * number of paths in G labeled with l
    uint32_t pairs  =0;  //number of distinct node pairs connected with paths labeled with l.
};
struct Syn2{
    uint32_t out    =0;  //number of nodes in G which have outgoing path labeled with l1/l2
    uint32_t middle =0;  // * number of nodes in G which have incoming edge labeled l1 and outgoing edge labeled l2,
    uint32_t in     =0;  // * number of nodes in G which have incoming path labeled with l1/l2
    uint32_t path   =0;  //number of paths in G labeled with l1/l2
    uint32_t pairs  =0;  //number of distinct node pairs connected with paths labeled with l1/l2.
    uint32_t one    =0;  //number of paths labeled l1 from nodes in out to nodes in middle, and
    uint32_t two    =0;  // * number of paths labeled l2 from nodes in middle to nodes in in.
};

class PathStatistic {

public:
    virtual ~PathStatistic() = default;
    void construct(const std::shared_ptr<SimpleGraph> &g);
    cardPathStat estimateConcat(cardPathStat left, cardPathStat right);
    cardPathStat estimateUnion(cardPathStat c1, cardPathStat c2);
    cardPathStat estimateGreater(uint32_t l);
    cardPathStat estimateLower(uint32_t l);
    cardPathStat estimateKleene(uint32_t l);

private:
    std::vector<Syn1> syn1;
    std::vector<std::vector<Syn2>> syn2; // for each pair of label (l1,l2), we can index syn2 by  | syn2[l1][l2] if l1 <= l2 
                                         //                                                       | syn2[l2][l1] if l1 >  l2
    u_int32_t diameter;
};

#endif //QUICKSILVER_PATHSTATISTIC_H
