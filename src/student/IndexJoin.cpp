//
// Created by Lucas Giordano on 09/04/2020.
//

#include "IndexJoin.h"

uint32_t IndexJoin::cost() {
    return 0;
}
IndexJoin::IndexJoin(Node V,PhysicalOperator *left, PhysicalOperator *right): PhysicalOperator(left, right, SOURCE_SORTED),V(V) {}

IndexJoin::~IndexJoin() {}

/////***********************************************************
Edge IndexJoin::produceNextEdge() {
    return out.pop();
}

void IndexJoin::skip(Node until) {
    throw "Unsupported operation";
}

std::ostream &IndexJoin::name(std::ostream &strm) const {
    return strm << "IndexJoin";
}
/////***********************************************************
/////***********************************************************


cardStat IndexJoin::eval() {
    std::thread thdLeft([this] {
        left->evalPipeline(SOURCE_SORTED);
    });
   /* right->evalPipeline(SOURCE_SORTED); //assumed to be fast : indexLookUp for instance
    std::vector<std::vector<Node>> adj(V, std::vector<Node>());
    for (Edge r = right->produceNextEdge(); r != END_EDGE ; r = right->produceNextEdge()){
        adj[r.source].emplace_back(r.target);
    }*/
    Edge l = left->produceNextEdge();
    std::vector<bool> hashIn(V, false);
    outCardStat = cardStat{0,0,0};
    while (l != END_EDGE){
        std::vector<Node> outputs;
        std::vector<Node> targets;
        Node s = l.source;
        while (l.source == s){
            targets.push_back(l.target);
            l = left->produceNextEdge();
        }
        for (auto m : targets){
            for (auto t : right->reachable(m)){
                outputs.push_back(t);
            }
        }
        std::sort(outputs.begin(),outputs.end());
        Node lastT = NONE;
        if (!outputs.empty()) outCardStat.noOut++;
        for (auto o : outputs){
            if (o != lastT){
                //out.push(Edge{s,o},false);
                lastT = o;
                outCardStat.noPaths++;
                if (hashIn[o] == false) {
                    hashIn[o] = true;
                    outCardStat.noIn++;
                }
            }
        }
    }
   // out.push(END_EDGE,true);
    left->terminate();
    right->terminate();
    thdLeft.join();
    return outCardStat;
}
void IndexJoin::evalPipeline(ResultSorted resultSorted) {
    switch(resultSorted){
        case TARGET_SORTED: evalPipelineRight();break;
        case ANY          :
        case NOT_SORTED   : //go in SourceSorted
        case SOURCE_SORTED: evalPipelineLeft();break;
    }
}


void IndexJoin::evalPipelineLeft() {
    std::thread thdLeft([this] {
        left->evalPipeline(SOURCE_SORTED);
    });
   /* right->evalPipeline(SOURCE_SORTED); //assumed to be fast : indexLookUp for instance
    std::vector<std::vector<Node>> adj(V, std::vector<Node>());
    for (Edge r = right->produceNextEdge(); r != END_EDGE ; r = right->produceNextEdge()){
        adj[r.source].emplace_back(r.target);
    }*/
    Edge l = left->produceNextEdge();

    while (l != END_EDGE){
        std::vector<Node> outputs;
        std::vector<Node> targets;
        Node s = l.source;
        while (l.source == s){
            targets.push_back(l.target);
            l = left->produceNextEdge();
        }
        for (auto m : targets){
            for (auto t : right->reachable(m)){
                outputs.push_back(t);
            }
        }
        std::sort(outputs.begin(),outputs.end());
        Node lastT = NONE;
        for (auto o : outputs){
            if (o != lastT){
                out.push(Edge{s,o},false);
                lastT = o;
            }
        }
    }
    out.push(END_EDGE,true);

    left->terminate();
    right->terminate();
    thdLeft.join();
}

void IndexJoin::evalPipelineRight() {
    std::thread thdRight([this] {
        right->evalPipeline(TARGET_SORTED);
    });
    left->evalPipeline(TARGET_SORTED); //assumed to be fast : indexLookUp for instance
    std::vector<std::vector<Node>> adj(V, std::vector<Node>());
    for (Edge l = left->produceNextEdge(); l != END_EDGE ; l = right->produceNextEdge()){
        adj[l.target].emplace_back(l.source);
    }
    Edge r = left->produceNextEdge();
    while (r != END_EDGE){
        std::vector<Node> outputs;
        std::vector<Node> sources;
        Node t = r.target;
        while (r.target == t){
            sources.push_back(r.source);
            r = right->produceNextEdge();
        }
        for (auto m : sources){
            for (auto t : adj[m]){
                outputs.push_back(t);
            }
        }
        std::sort(outputs.begin(),outputs.end());
        Node lastS = NONE;
        for (auto o : outputs){
            if (o != lastS){
                out.push(Edge{o,t},false);
                lastS = o;
            }
        }
    }
    out.push(END_EDGE,true);

    left->terminate();
    right->terminate();
    thdRight.join();
}



