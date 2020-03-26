//
// Created by Lucas Giordano on 15/03/2020.
//

#ifndef QUICKSILVER_BLOCKINGQUEUE_H
#define QUICKSILVER_BLOCKINGQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>

template<typename Data>
class BlockingQueue {
    private:
        std::queue<Data> queue;
        std::mutex  mtx;
        std::condition_variable pushCond;
        std::condition_variable popCond;
        size_t limit;
        size_t signalingPushSize;
    public:
        /**
         * Constructor
         */
        BlockingQueue(bool fastProducer = true,size_t limit = 200) : limit(limit),queue(), mtx(), pushCond(),popCond() {
            assert(limit > 0);
            signalingPushSize = (fastProducer)? std::max(limit/2, (size_t)1): 1;
        }

        /**
        * Push an Edge into the queue q in a thread safe way
         * */
        void push(Data elem, bool lastPush)  {
            std::unique_lock<std::mutex> lock(mtx);
            while (queue.size() >= limit) {
                pushCond.wait(lock);
            }
            assert(queue.size() < limit);
            queue.push(elem);
            if (queue.size() >= std::max(limit/5,(size_t)1) || lastPush) popCond.notify_one();
        };
        /**
        * Pop an Edge from the queue q in a thread safe way
        * */
        Data pop() {
            std::unique_lock<std::mutex> lock(mtx);
            while (queue.empty()){
                popCond.wait(lock);
            }
            assert(queue.size() > 0);
            Data elem = queue.front();
            queue.pop();
            if (queue.size() >= signalingPushSize) pushCond.notify_one();
            return elem;
        };
        /**
         * clear the queue
         **/
         void clear(){
            std::unique_lock<std::mutex> lock(mtx);
            std::queue<Data> empty;
            std::swap( queue, empty );
            pushCond.notify_one();
         };
         /**
          * @return size of the queue
          */
         size_t size(){
             return queue.size();
         };
};

#endif //QUICKSILVER_BLOCKINGQUEUE_H
