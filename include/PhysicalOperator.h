//
// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_PHYSICALOPERATOR_H
#define QUICKSILVER_PHYSICALOPERATOR_H

#include <queue>
#include <vector>
#include "Edge.h"
#include "Estimator.h"
#include <mutex>
#include <unordered_map>
#include "BlockingQueue.h"
#include <thread>

typedef BlockingQueue<OutEdge> Queue;
static OutEdge END_EDGE = OutEdge{NONE, NONE};

enum ResultSorted {
    SOURCE_SORTED, TARGET_SORTED, NOT_SORTED
};

class PhysicalOperator {
protected:
     Queue out;
     PhysicalOperator* left;
     PhysicalOperator* right;
     ResultSorted resultSorted;
     /**
      *  Interface linking the parent operator and the child.
      **/
     virtual OutEdge produceNextEdge() = 0;
    /**
     * Evaluates the physical operator in a pipelining fashion
     * */
    virtual void evalPipeline() = 0;
public:
    /**
     * Constructor
     **/
    PhysicalOperator(PhysicalOperator* left,PhysicalOperator* right, ResultSorted resultSorted) :
        out(), left(left), right(right), resultSorted(resultSorted) {}

    /**
     * Returns the cost of the physical operator
     * */
    virtual uint32_t cost() const = 0;
    /**
     * Tests if this operator is a leaf
     **/
    bool isLeaf() {
        return left == nullptr && right == nullptr;
    }
    /**
     * Destructor
     **/
    virtual ~PhysicalOperator() {
        delete(left);
        delete(right);
    };
    /**
     * Returns true the physical operator represents a bounded left query (Node,l,*)
     * */
    virtual bool isLeftBounded() const {
        return (left != nullptr)?left->isLeftBounded():false;
    };
    /**
     * Returns true the physical operator represents a bounded right query (*,l,Node)
     * */
    virtual bool isRightBounded() const{
        return (right != nullptr)?right->isLeftBounded():false;
    };

    /**
     * /!\ MACRO /!\
     *
     * Generic function used to update the cardSat result in eval
     **/
    #define update(elem, lastElem, hashTable, sorted, countVar) {                                                                   \
            if      ( (sorted) && !((elem) == (lastElem)))                     {(countVar)++; (lastElem) = (elem);}                 \
            else if (!(sorted) && (hashTable).find(elem) == (hashTable).end()) {(countVar)++; (hashTable).insert({(elem),(elem)});} \
    }
    /**
     * Evaluates completely the physical operator and returns :
     *          cardStat {
     *              noOut,
     *              noPaths,
     *              noIn
     *          }
     * Since we require the out queue to have items sorted by target value, we can easily compute both
     *          * noIn
     *          * noPaths
     * To compute noOut, we will construct a hash table
     *
     * /!\ To speed up the computation we can also use isLeftBounded() and isRightBounded() /!\
     * */
     cardStat eval() {
         std::thread thd([this] {
            evalPipeline();
         });

         uint32_t noOut  = 0;
         uint32_t noPath = 0;
         uint32_t noIn   = 0;

         uint32_t lastIn  = NONE;     std::unordered_map<uint32_t,uint32_t> hashIn;    bool sortedIn   = resultSorted == TARGET_SORTED;
         uint32_t lastOut = NONE;     std::unordered_map<uint32_t,uint32_t> hashOut;   bool sortedOut  = resultSorted == SOURCE_SORTED;
         OutEdge lastEdge = END_EDGE; std::unordered_map<OutEdge,OutEdge,HashOutEdge> hashEdge; bool sortedPath = sortedIn || sortedOut;

         for (OutEdge e = produceNextEdge() ; !(e == END_EDGE); e = produceNextEdge()){
             update(e.target,lastIn,hashIn, sortedIn, noIn);
             update(e.source,lastOut,hashOut,sortedOut, noOut);
             update(e,lastEdge,hashEdge,sortedPath, noPath);

         }
         thd.join();  //terminate execution of evalPipeline()

         if (isLeftBounded()) assert(noOut <= 1);
         if (isRightBounded()) assert(noIn <= 1);
         return cardStat{noOut,noPath,noIn};
     };

};


#endif //QUICKSILVER_PHYSICALOPERATOR_H
