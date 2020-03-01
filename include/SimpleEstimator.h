#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"

enum op_t {greater,lower,kleene};

/**
 * Adapter structure of cardStar used to store extra information used in the concat method :
 *              - l      :  the label from which the this cardStat has been computed
 *              - lastOp :  the last operation main that lead to this cardStat in the PathTree.
 *                          At a concat node we always take the operation of the righ tree for
 *                          the result of the concatenation.
 * */
struct cardPathStat{
    op_t lastOp;
    uint32_t l;
    cardStat stat;
};
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
    uint32_t in  =0;  // * number of nodes in G which have incoming edge labeled l1 and incoming edge labeled l2,
    uint32_t twoIn =0; //* number of path from nodes in syn1[l1].out to nodes in syn3[l1][l2].in
    uint32_t out =0; // * number of nodes in G which have outgoing edge labeled l1 and outgoing edge labeled l2,
    uint32_t twoOut =0; //* number of path from nodes in syn1[l1].in to nodes in syn3[l1][l2].out
};

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override ;
    cardStat estimate(PathQuery *q) override ;

private:
    void printGraphInfo();
    cardPathStat estimateLeaf(std::string reg_exp);
    cardPathStat estimatePathTree(PathTree *p);

    //PathStatistic methodes and attributes
    static bool sortPairsL(const std::pair<uint32_t,uint32_t> &a, const std::pair<uint32_t,uint32_t> &b);
    cardPathStat estimateConcat(cardPathStat left, cardPathStat right);
    cardPathStat estimateUnion(cardPathStat c1, cardPathStat c2);
    cardPathStat estimateGreater(uint32_t l);
    cardPathStat estimateLower(uint32_t l);
    cardPathStat estimateKleene(uint32_t l);

    std::vector<Syn1> syn1;
    std::vector<std::vector<Syn2>> syn2;
    std::vector<std::vector<Syn3>> syn3;
    const uint32_t ESTIMATED_AVG_PATH_LENGTH = 3; //used by Kleene Start method
};


#endif //QS_SIMPLEESTIMATOR_H
