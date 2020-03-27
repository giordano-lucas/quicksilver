#include <cmath>
#include <student/PhysicalOperator.h>
#include <student/MergeJoin.h>
#include <student/KleeneStar.h>
#include <student/IndexLookUp.h>
#include <student/HelperPhysicalOperator.h>
#include "SimpleGraph.h"
#include "SimpleEstimator.h"



SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &ind){
    // works only with SimpleGraph
    index = ind;
}


#define count(elem, hashTable, countVar) { \
        if ((hashTable).find(elem) == (hashTable).end()) {(countVar)++; (hashTable).insert({(elem),(elem)});} \
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
cardStat eval(std::vector<Edge> edges) {
    uint32_t noOut  = 0;
    uint32_t noPath = 0;
    uint32_t noIn   = 0;
    std::unordered_map<uint32_t,uint32_t> hashIn;
    std::unordered_map<uint32_t,uint32_t> hashOut;
    std::unordered_map<Edge,Edge,HashEdge> hashEdge;
    for (Edge e : edges){
        // std::cout << "PHY : " << e;
        count(e.target,hashIn, noIn);
        count(e.source,hashOut, noOut);
        count(e,hashEdge, noPath);
    }
    return cardStat{noOut,noPath,noIn};
};



// sort on the second item in the pair, then on the first (ascending order)

#define targetL(t) ((revL)?((t)->first).source:((t)->first).target)
#define targetR(t) ((revR)?((t)->first).source:((t)->first).target)
#define sourceR(t) ((revR)?((t)->first).target:((t)->first).source)
#define getEdgeL(t) ((revL)?(reverse((t)->first)):((t)->first))
#define getEdgeR(t) ((revR)?(reverse((t)->first)):((t)->first))
cardStat join(IndexResult left, IndexResult right, bool revL, bool revR,
        std::unordered_map<Node,bool> &inMap,
        std::unordered_map<Node,bool> &middleMap,
        std::unordered_map<Edge,Node,HashEdge> &twoMap){
    std::vector<Edge> res;
    auto r = right.first;
    auto l = left.first;
    while(r!=right.second && l != left.second){
        while (l!=left.second && targetL(l) < sourceR(r)) { l++; } //advance until we have a joining edge
        Edge tl = getEdgeL(l);
        bool done = false;
        while  (!done && l != left.second){ //setL contains all edges in Left that have the same target value
            if (tl.target == targetL(l)){l++;}
            else done = true;
        }
        Edge valR =  getEdgeR(r);
        while (r!=right.second && sourceR(r) < tl.target) { r++;} //advance until we have a joining edge
        if    (r!=right.second && tl.target == sourceR(r))  middleMap.insert({tl.target,true}); //have a middle
        while (r!=right.second && tl.target == sourceR(r)){
            twoMap.insert({getEdgeR(r),true}); //have a two
            inMap.insert({targetR(r),true});  //have an in
            r++;
        }
    }
    return eval(res);
}


void SimpleEstimator::prepare() {
    /*
    std::unordered_map<Node,bool> inMap;
    std::unordered_map<Node,bool> middleMap;
    std::unordered_map<Edge,Node,HashEdge> twoMap;
    for (Label l1 = 0 ; l1 < index->getNoLabels() ; ++l1){
        for (Label l2 = 0 ; l2 < index->getNoLabels() ; ++l2){
            //============= index->syn2 =================
            inMap.clear();
            middleMap.clear();
            twoMap.clear();
            IndexResult left  = index->getEdgesTarget(QueryEdge{NONE,l1,NONE});
            IndexResult right = index->getEdgesSource(QueryEdge{NONE,l2,NONE});
            cardStat res = join(left,right,true,false,inMap,middleMap,twoMap);
            index->syn2[l1][l2].in     = inMap.size();
            index->syn2[l1][l2].middle = middleMap.size();
            index->syn2[l1][l2].two    = twoMap.size();
            //============= index->syn3 and 4 =================
            //--- Part <> ---
            inMap.clear();
            middleMap.clear();
            twoMap.clear();
            left  = index->getEdgesSource(QueryEdge{NONE,l1,NONE});
            right = index->getEdgesSource(QueryEdge{NONE,l2,NONE});
            res =  join(left,right,true,false,inMap,middleMap,twoMap);
            index->syn4[l1][l2].out       = inMap.size();
            index->syn4[l1][l2].middleOut = middleMap.size();
            index->syn4[l1][l2].twoOut    = twoMap.size();
            //--- Part >< ---
            inMap.clear();
            middleMap.clear();
            twoMap.clear();
            left  = index->getEdgesTarget(QueryEdge{NONE,l1,NONE});
            right = index->getEdgesTarget(QueryEdge{NONE,l2,NONE});
             res =  join(left,right,true,false,inMap,middleMap,twoMap);
             //if (l1 == 0 && l2 == 1) assert(res.noOut == 2886 && res.noPaths == 183286 && res.noIn == 2547);
            index->syn3[l1][l2].in       = inMap.size();
            index->syn3[l1][l2].middleIn = middleMap.size();
            index->syn3[l1][l2].twoIn    = twoMap.size();
        }
    }*/
}

std::string printQuery(basic_query_t q){
    switch (q.op){
        case kleene : return "+";
        case greater: return ">";
        case lower  : return "<";
        case select : return "sel";
    }
}


cardStat SimpleEstimator::estimatePhy(void *op) {
    PhysicalOperator* o = static_cast<PhysicalOperator *>(op);
    query_t q = o->flatten();
    bool boundedRight = false;
    bool boundedLeft = false;
    if (q.back().op == select){
        boundedRight = true;
        q.pop_back();
    }
    auto it = q.begin();
    if (q.front().op == select){
        boundedLeft = true;
        it++;
    }
    cardStat card = estimateSimple(*it);
    auto lastOp = *it;
    it++;
    for (;it != q.end(); it++){
        card = estimateConcat(card,lastOp,estimateSimple(*it), *it);
        lastOp = *it;
    }

    //selection operation
    if (!boundedLeft && !boundedRight) return card;
    else if (boundedLeft)  return cardStat{
                                      1,
                                      static_cast<uint32_t>(std::ceil(((double) (card.noPaths)) / card.noOut)),
                                      static_cast<uint32_t>(std::ceil(((double) (card.noIn)) / card.noOut))
                                      };
    else if (boundedRight) return  cardStat{
                                        static_cast<uint32_t>(std::ceil(((double) (card.noOut))/ card.noIn)),
                                        static_cast<uint32_t>(std::ceil(((double) (card.noPaths)) / card.noIn)),
                                        1
                                        };
    throw "error cannot have both bounded source and target query (est)";
}

cardStat SimpleEstimator::estimateSimple(basic_query_t q){
    switch (q.op){
        case kleene : return estimateKleene(q.value);
        case greater: return estimateGreater(q.value);
        case lower  : return estimateLower(q.value);
    }
    throw "error operator not known estimate simple";
}

/* /!\ ONLY VALID FOR LEFT-DEEP PATH TREE /!\*/
cardStat SimpleEstimator::estimateConcat(cardStat left, basic_query_t qL, cardStat right, basic_query_t qR){
    uint32_t l1 = qL.value;
    uint32_t l2 = qR.value;
    cardStat res;
    if ((qL.op == greater && qR.op == lower)){
        res =  cardStat{
                static_cast<uint32_t>(left.noOut *(((double)index->syn3[l1][l2].middleIn) / index->syn1[l1].in)),
                static_cast<uint32_t>((((double)left.noPaths * index->syn3[l1][l2].twoIn) / index->syn1[l1].in)) ,
                static_cast<uint32_t>(left.noOut *(((double)index->syn3[l1][l2].in) / index->syn1[l1].in))
        };
    }
    else if ((qL.op == lower && qR.op == greater)){
        res =  cardStat{
                static_cast<uint32_t>(left.noOut *(((double)index->syn4[l1][l2].middleOut) / index->syn1[l1].out)),
                static_cast<uint32_t>((((double)left.noPaths * index->syn4[l1][l2].twoOut) / index->syn1[l1].out)) ,
                static_cast<uint32_t>(left.noOut *(((double)index->syn4[l1][l2].out) / index->syn1[l1].out))
        };
    }
    else {
       if (qL.op == lower && qR.op == lower){ //if they are both < operation we are looking for path s->l2->l1->t instead of s->l1->l2->t
            l1 = qR.value;
            l2 = qL.value;
        }
        res =  cardStat{
                static_cast<uint32_t>(left.noOut * (((double)index->syn2[l1][l2].middle )/ index->syn1[l1].in)),
                static_cast<uint32_t>(left.noPaths * (((double)index->syn2[l1][l2].two) / index->syn1[l1].in)),
                static_cast<uint32_t>(left.noIn * (((double)index->syn2[l1][l2].in) / index->syn1[l1].in))
        };
    }
    return res;
}

cardStat SimpleEstimator::estimateGreater(uint32_t l) {
    return cardStat{index->syn1[l].out, index->syn1[l].path, index->syn1[l].in};
}
cardStat SimpleEstimator::estimateLower(uint32_t l){
    return cardStat{index->syn1[l].in, index->syn1[l].path, index->syn1[l].out};
}
cardStat SimpleEstimator::estimateUnion(cardStat left, cardStat right){
    cardStat min = (left.noPaths <= right.noPaths)? left: right;
    cardStat max = (left.noPaths > right.noPaths)? left: right;
    cardStat res = cardStat{max.noOut + min.noOut/2, max.noPaths + min.noPaths/2, max.noIn + min.noIn/2};
    return res;
}
cardStat SimpleEstimator::estimateKleene(uint32_t l){
    cardStat path1 = estimateGreater(l); //path of length 1
    cardStat res   = path1;
    for (uint32_t i = 0 ; i < ESTIMATED_AVG_PATH_LENGTH ; ++i){
        res = estimateUnion(res, estimateConcat(res, basic_query_t{greater,l},path1,basic_query_t{greater,l}));
    }
    return res;
}
//*******************************************************

cardStat SimpleEstimator::estimate(PathQuery *q) {
    PhysicalOperator* op = ofPathQuery(q,index);
    return estimatePhy(op);
}
