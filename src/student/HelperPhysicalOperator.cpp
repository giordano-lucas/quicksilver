//
// Created by Lucas Giordano on 27/03/2020.
//
#include "HelperPhysicalOperator.h"
#include <student/IndexLookUp.h>
#include <student/KleeneStar.h>
#include <student/MergeJoin.h>
#include <student/IndexJoin.h>
#include <student/IndexLookUp2.h>


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
     //                        ofPathTree(tree->right, index, NONE, rightBounded));
      //  return new MergeJoin(ofPathTree(tree->left, index, leftBounded, NONE),
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




//**************************************************************************************
PhysicalOperator* ofPathTreeLargeCard(PathTree* tree, std::shared_ptr<SimpleGraph>& index, Node leftBounded, Node rightBounded,bool allowIndexLookUp2){
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
        if (allowIndexLookUp2 && (tree->right->isLeaf() || tree->left->isLeaf() ||tree->left->right->isLeaf())){ // if we can use < < indexLookUp
            std::regex directLabel(R"((\d+)\>)");
            std::regex inverseLabel(R"((\d+)\<)");
            std::regex kleeneStar(R"((\d+)\+)");

            std::smatch matches;
            std::smatch matches2;
            if (std::regex_search(tree->right->data, matches, directLabel)) {
                Label l2 = (uint32_t) std::stoul(matches[1]);
                if (std::regex_search( (tree->left->isLeaf())?tree->left->data:tree->left->right->data, matches2, directLabel)){
                    Label l1 = (uint32_t) std::stoul(matches2[1]);

                    if (tree->left->isLeaf()) return new IndexLookUp2(index, l1,l2);
                    else                      return new IndexJoin(index->getNoVertices(),
                            ofPathTreeLargeCard(tree->left->left, index, leftBounded, NONE,false),
                                                                   new IndexLookUp2(index, l1,l2));

                }
            }
        }
        return new IndexJoin(index->getNoVertices(),ofPathTreeLargeCard(tree->left, index, leftBounded, NONE,false),
                             ofPathTreeLargeCard(tree->right, index, NONE, rightBounded,false));
    }
    throw "Illegal argument";
}
PhysicalOperator* ofPathQueryLargeCard(PathQuery* pq, std::shared_ptr<SimpleGraph>& index) {
    Node s = (pq->s.compare("*")==0)?NONE: (uint32_t) std::stoi(pq->s);
    Node t = (pq->t.compare("*")==0)?NONE: (uint32_t) std::stoi(pq->t);
    //std::cout << "(" << s << "," << t<< ")\n";
    return ofPathTreeLargeCard(pq->path, index,s,t, false);
    //*******************************************************************************
  //  return ofPathTreeLargeCard(pq->path, index,s,t, true);
    //*******************************************************************************
}