//
// Created by Lucas Giordano on 12/03/2020.
//

#ifndef QUICKSILVER_PHYSICALOPERATOR_H
#define QUICKSILVER_PHYSICALOPERATOR_H

#include <queue>
#include <vector>
#include "Edge.h"
#include <mutex>
typedef std::queue<std::vector<Edge>> Queue;
class PhysicalOperator {
     Queue* out;
     Queue* inLeft;
     Queue* inRight;
     std::mutex* mutexOut;
     std::mutex* mutexInLeft;
     std::mutex* mutexInRight;

public:
    PhysicalOperator(Queue *out, Queue *inLeft, Queue *inRight, std::mutex *mutexOut, std::mutex *mutexInLeft,
                     std::mutex *mutexInRight) : out(out), inLeft(inLeft), inRight(inRight), mutexOut(mutexOut),
                                                 mutexInLeft(mutexInLeft), mutexInRight(mutexInRight) {}

    virtual void eval() const = 0;
    virtual uint32_t cost() const = 0;
};


#endif //QUICKSILVER_PHYSICALOPERATOR_H
