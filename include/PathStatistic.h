
#ifndef QUICKSILVER_PATHSTATISTIC_H
#define QUICKSILVER_PATHSTATISTIC_H
#include <memory>
#include "SimpleGraph.h"
#include "Estimator.h"
#include <cstdint>

struct Syn1{
    uint32_t l;
    uint32_t out;    //number of nodes in G which have outgoing edge labeled with l
    uint32_t in;     //number of nodes in G which have incoming edge labeled with l
    uint32_t path;   //number of paths in G labeled with l
    uint32_t pairs;  //number of distinct node pairs connected with paths labeled with l.
};
struct Syn2{
    uint32_t l1;
    uint32_t l2;
    uint32_t out;    //number of nodes in G which have outgoing path labeled with l1/l2
    uint32_t middle; // number of nodes in G which have incoming edge labeled l1 and outgoing edge labeled l2,
    uint32_t in;     //number of nodes in G which have incoming path labeled with l1/l2
    uint32_t path;   //number of paths in G labeled with l1/l2
    uint32_t pairs;  //number of distinct node pairs connected with paths labeled with l1/l2.
};
class PathStatistic {

public:
    virtual ~PathStatistic() = default;
    void construct(const std::shared_ptr<SimpleGraph> &g);
    PathStatistic estimateConcat(PathStatistic p);
    PathStatistic estimateUnion(PathStatistic p);
    PathStatistic estimateGreater(uint32_t l);
    PathStatistic estimateLower(uint32_t l);
    PathStatistic estimateKleene(uint32_t l);
    cardStat toCardStat();
private:
    std::vector<Syn1> syn1;
    std::vector<Syn2> syn2;
    u_int32_t diameter;
};

#endif //QUICKSILVER_PATHSTATISTIC_H
