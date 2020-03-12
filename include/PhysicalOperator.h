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
#include "unordered_map"

typedef std::queue<Edge*> Queue;
static Edge END_EDGE = Edge{NONE, NONE, NONE};

class PhysicalOperator {
protected:
     Queue* out;
     Queue* inLeft;
     Queue* inRight;
     std::mutex* mutexOut;
     std::mutex* mutexInLeft;
     std::mutex* mutexInRight;
     PhysicalOperator* left;
     PhysicalOperator* right;
    /**
     * Push an Edge into the queue q in a thread safe way
     * */
     void push(Queue* q, std::mutex* m, Edge* e) const {
         m->lock();
         q->push(e);
         m->unlock();
     };
    /**
    * Pop an Edge from the queue q in a thread safe way
    * */
     Edge* pop(Queue* q, std::mutex* m) const{
        m->lock();
        Edge* e = q->front();
        q->pop();
        m->unlock();
        return e;
     };

public:
    /**
     * Constructor
     **/
    PhysicalOperator(Queue *out, Queue *inLeft, Queue *inRight, std::mutex *mutexOut, std::mutex *mutexInLeft,
                     std::mutex *mutexInRight, PhysicalOperator* left,PhysicalOperator* right) : out(out), inLeft(inLeft), inRight(inRight), mutexOut(mutexOut),
                                                 mutexInLeft(mutexInLeft), mutexInRight(mutexInRight), left(left), right(right) {}
    /**
     * Evaluates the physical operator in a pipelining fashion
     * */
    virtual void evalPipeline() const = 0;
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
    cardStat eval() const {
        uint32_t noOut  = 0;
        uint32_t noPath = 0;
        uint32_t noIn   = 0;

        uint32_t lastIn = NONE;
        Edge lastEdge   = END_EDGE;
        std::unordered_map<uint32_t,uint32_t> hashOut;
        for (Edge* e = pop(out,mutexOut) ; !((*e) == END_EDGE); e = pop(out, mutexOut)){
            if (e->target != lastIn)                      {noIn++  ; lastIn   = e->target;}
            if (!((*e) == lastEdge))                      {noPath++; lastEdge = *e;}
            if (hashOut.find(e->source) != hashOut.end()) {noOut++ ; hashOut.insert({e->source,e->source});}
        }
        if (isLeftBounded()) assert(noOut ==1);
        if (isRightBounded()) assert(noIn ==1);
        return cardStat{noOut,noPath,noIn};
    };
};


#endif //QUICKSILVER_PHYSICALOPERATOR_H
