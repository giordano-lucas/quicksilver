//
// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_PHYSICALOPERATOR_H
#define QUICKSILVER_PHYSICALOPERATOR_H

#include <queue>
#include <vector>
#include "Edge.h"
#include <mutex>
#include <unordered_map>
#include "BlockingQueue.h"
#include <thread>
#include <ostream>
#include <iostream>
#include <SimpleEstimator.h>
#include "SimpleEvaluator.h"

static Edge END_EDGE = Edge{NONE, NONE};

enum ResultSorted {
    SOURCE_SORTED, TARGET_SORTED, NOT_SORTED, ANY
};

class PhysicalOperator {
protected:
     BlockingQueue<Edge> out;
     PhysicalOperator* left;
     PhysicalOperator* right;
     bool terminated = false;
     std::shared_ptr<SimpleEstimator> est;
     cardStat card = cardStat{NONE,NONE,NONE};
     query_t query;
     cardStat outCardStat = cardStat{NONE,NONE,NONE};
public:
    ResultSorted defaultResultSorted;
    ResultSorted resultSorted = NOT_SORTED;
    /**
     * Constructor
     **/
    PhysicalOperator(PhysicalOperator* left,PhysicalOperator* right,ResultSorted defaultResultSorted) :
        out(), left(left), right(right),defaultResultSorted(defaultResultSorted) {

        if (left != nullptr && right != nullptr){
            for (auto q : left->flatten()) query.push_back(q);
            for (auto q : right->flatten()) query.push_back(q);
        }

    }
    /**
     * Terminate the computation of this physical operator
     */
     void terminate() {
         out.clear();
         terminated = true;
     };
    /**
     * Returns the cost of the physical operator
     * */
    virtual uint32_t cost() = 0;
    /**
     * Tests if this operator is a leaf
     **/
    bool isLeaf() const{
        return left == nullptr && right == nullptr;
    };
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
     *
     **/
    void attachEstimator(std::shared_ptr<SimpleEstimator> &est) {
        PhysicalOperator::est = est;
        if (left != nullptr) left->attachEstimator(est);
        if (right != nullptr) right->attachEstimator(est);
    };
    /**
     * Returns the cardinality estimation of the result of this physical operator
     **/
    cardStat getCardinality() {
        if (card.noPaths == NONE) card = est->estimate(flatten());
        return card;
    };
    query_t  flatten() const {
        return query;
    }

    /**
     * /!\ MACRO /!\
     *
     * Generic function used to update the cardSat result in eval
     **/
    #define update(elem, lastElem, hashTable, sorted, countVar) {                                                                   \
            if      ( (sorted) && !((elem) == (lastElem)))                     {(countVar)++; (lastElem) = (elem);}                 \
            else if (!(sorted) && (hashTable).find(elem) == (hashTable).end()) {(countVar)++; (hashTable).insert({(elem),true});}   \
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
     virtual cardStat eval() {
         std::thread thd([this] {
            evalPipeline(defaultResultSorted);
         });
         Edge e = produceNextEdge();
         if (outCardStat.noPaths != NONE) {
             thd.join();  //terminate execution of evalPipeline()
             return outCardStat;
         }

         uint32_t noOut  = 0;
         uint32_t noPath = 0;
         uint32_t noIn   = 0;

         uint32_t lastIn  = NONE;     std::unordered_map<uint32_t,bool> hashIn;    bool sortedIn   = defaultResultSorted == TARGET_SORTED;
         uint32_t lastOut = NONE;     std::unordered_map<uint32_t,bool> hashOut;   bool sortedOut  = defaultResultSorted == SOURCE_SORTED;
         Edge lastEdge = END_EDGE; std::unordered_map<Edge,bool,HashEdge> hashEdge; bool sortedPath = sortedIn || sortedOut;

         for (;!(e == END_EDGE); e = produceNextEdge()){
             //std::cout << "PHY : " << e;
             update(e.target,lastIn,hashIn, sortedIn, noIn);
             update(e.source,lastOut,hashOut,sortedOut, noOut);
             update(e,lastEdge,hashEdge,sortedPath, noPath);
         }
         thd.join();  //terminate execution of evalPipeline()

        // if (isLeftBounded()) assert(noOut <= 1);
         //if (isRightBounded()) assert(noIn <= 1);
         return cardStat{noOut,noPath,noIn};
     };

    /**
     * Evaluates the physical operator in a pipelining fashion
     * */
    virtual void evalPipeline(ResultSorted resultSorted) = 0;

    /**
     *  Interface linking the parent operator and the child.
     **/
    virtual Edge produceNextEdge() = 0;
    virtual void skip(Node until) = 0;
    virtual std::vector<Node>& reachable(Node s) {
        throw "unimplemented";
    }
    virtual std::ostream& name(std::ostream &strm) const = 0;

    friend std::ostream &operator<<(std::ostream &strm, const PhysicalOperator &op) {
        if (op.isLeaf()) return op.name(strm);
        else                    op.name(strm) << ":" << *op.left << "," << *op.right;
        strm << "\n";
        return strm;
    };
};




#endif //QUICKSILVER_PHYSICALOPERATOR_H
