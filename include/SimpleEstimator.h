#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"


enum op_t {greater,lower,kleene, selection};
typedef struct {
    op_t op;
    uint32_t  value;
} basic_query_t;
typedef std::vector<basic_query_t> query_t;

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

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> index;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override ;
    cardStat estimate(PathQuery *q) override;
    cardStat estimate(query_t q);

private:
    cardStat estimateSimple(basic_query_t q);
    cardStat estimateConcat(cardStat left, basic_query_t qL, cardStat right, basic_query_t qR, bool firstConcat);
    cardStat estimateUnion(cardStat c1, cardStat c2);
    cardStat estimateGreater(uint32_t l);
    cardStat estimateLower(uint32_t l);
    cardStat estimateKleene(uint32_t l);
    const uint32_t ESTIMATED_AVG_PATH_LENGTH = 3; //used by Kleene Start method
};


#endif //QS_SIMPLEESTIMATOR_H
