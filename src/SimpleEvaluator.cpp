#include <PhysicalOperator.h>
#include <IndexLookUp.h>
#include <KleeneStar.h>
#include <MergeJoin.h>
#include "SimpleEstimator.h"
#include "SimpleEvaluator.h"

SimpleEvaluator::SimpleEvaluator(std::shared_ptr<SimpleGraph> &g) {

    // works only with SimpleGraph
    graph = g;
    est = nullptr; // estimator not attached by default
}

void SimpleEvaluator::attachEstimator(std::shared_ptr<SimpleEstimator> &e) {
    est = e;
}

void SimpleEvaluator::prepare() {

    // if attached, prepare the estimator
    if(est != nullptr) est->prepare();

    // prepare other things here.., if necessary

}


PhysicalOperator* ofPathTree(PathTree* tree, std::shared_ptr<SimpleGraph>& index, Node leftBounded, Node rightBounded){
    if (tree->isLeaf()){
        std::regex directLabel(R"((\d+)\>)");
        std::regex inverseLabel(R"((\d+)\<)");
        std::regex kleeneStar(R"((\d+)\+)");

        std::smatch matches;
        uint32_t label;
        bool inverse;
        if (std::regex_search(tree->data, matches, directLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            return new IndexLookUp(index,QueryEdge{leftBounded,label,rightBounded},false);
        } else if (std::regex_search(tree->data, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            return  new IndexLookUp(index,QueryEdge{leftBounded,label,rightBounded},true);
        }
        else if(std::regex_search(tree->data, matches, kleeneStar)) {
            label = (uint32_t) std::stoul(matches[1]);
            return new KleeneStar(index,QueryEdge{leftBounded,label,rightBounded});
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
        }
    }
    else if (tree->isConcat()){
        return new MergeJoin(ofPathTree(tree->left, index, leftBounded, NONE),
                             ofPathTree(tree->right, index, NONE, rightBounded));
    }
    throw "Illegal argument";
}
PhysicalOperator* ofPathQuery(PathQuery* pq, std::shared_ptr<SimpleGraph>& index) {
    Node s = (pq->s.compare("*")==0)?NONE: (uint32_t) std::stoi(pq->s);
    Node t = (pq->t.compare("*")==0)?NONE: (uint32_t) std::stoi(pq->t);
    //std::cout << "(" << s << "," << t<< ")\n";
    return ofPathTree(pq->path, index,s,t);
}
cardStat SimpleEvaluator::evaluate(PathQuery *query) {
    PhysicalOperator* op = ofPathQuery(query, graph);
    auto res = op->eval();
    delete op;
    return res;
}
