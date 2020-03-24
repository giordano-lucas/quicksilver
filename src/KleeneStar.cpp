//
// Created by Lucas Giordano on 24/03/2020.
//


#include "KleeneStar.h"

uint32_t KleeneStar::cost() const {
    return getCardinality().stat.noPaths;
}

cardPathStat KleeneStar::getCardinality() const {
    return cardPathStat();
}
/**
 * Use a dfs to compute all nodes reachable form the node source
 *
 * @param base        : (adjency list representation based on a hash table allowing muliple values for a given key) graph representation
 * @param discovered  : data structure (hash table) to store which vertices have already been explored
 * @param n           : explored node of the dfs algorithm
 * @param source      : vertex from which we want to compute the set of reachable nodes
 * @param reversed    : need to reverse the edges push into the out queue
 */
void KleeneStar::dfs(std::unordered_multimap<Node,Node>& base, std::unordered_map<Node,bool>& discovered,Node n, Node source, bool reversed){
    discovered.insert({n,true});//set vertex n to discovered
    //auto t = base.equal_range(n);
    auto t = index->getEdgesSource(QueryEdge{n,queryEdge.label,NONE});
    for (auto it = t.first ; it!=t.second; ++it){     //explore all nodes in base.adj[n]
       // Node target = it->second;
       Node target = it->first.target;
        if (discovered.find(target) == discovered.end()){    //if target has not been explored yet => explore
            out.push((reversed)?Edge{target,source}:Edge{source,target},false); //(source,target) is part of the results of the physical operator
            dfs(base,discovered,target,source,reversed);                                      //explore
        }
    }
}
/**
 * Kleene star based on DFS to allow the projection operation to be pushed down to the leaves
 * Complexity :
 *          - O((E+V) * V) if queryEdge  = (*,l,*)
 *          - O(E + V)     if queryEdge  = (n,l,*) or (*,l,n)
 */
void KleeneStar::evalPipeline() {
    IndexResult res = index->getEdgesSource(QueryEdge{NONE,queryEdge.label,NONE}); //get data from the database
    std::unordered_multimap<Node,Node> base;  //graph adj list representation
    std::map<Node,bool> setNodes;             //set of nodes sorted
    std::unordered_map<Node,bool> discovered;
    /*
    if (queryEdge.target == NONE) {
        for (IndexIterator it = res.first; it != res.second; ++it) {
         //   base.insert({it->first.source, it->first.target}); //regular adj list
            setNodes.insert({it->first.source, false});
            setNodes.insert({it->first.target, false});
        }
    }
    else {
        for (IndexIterator it = res.first; it != res.second; ++it) {
          //  base.insert({it->first.target, it->first.source}); //reverse adj list
            setNodes.insert({it->first.source, false});
            setNodes.insert({it->first.target, false});
        }
    }
    */

    if      (queryEdge.source != NONE) dfs(base,discovered,queryEdge.source,queryEdge.source, false);
    else if (queryEdge.target != NONE) dfs(base,discovered,queryEdge.target,queryEdge.target, true); //in reverse adj list => need to re-reverse the edges at the end
    else {  //if the query is not bounded (*,l,*) => perform dfs for all sources nodes
        /*for (auto it = setNodes.begin(); it != setNodes.end(); ++it) {
           // std::cout << "Kleene : " << it->first << "\n";
            Node key = it->first;
            discovered.clear();
            dfs(base,discovered,key,key,false);

        }*/
        Node lastSource = NONE;
        for (auto it = res.first ; it != res.second; ++it){
            if (it->first.source != NONE){
                discovered.clear();
                dfs(base,discovered,it->first.source,it->first.source,false);
            }
        }
    }
    out.push(END_EDGE,true);
}

Edge KleeneStar::produceNextEdge() {
    return out.pop();
}

std::ostream &KleeneStar::name(std::ostream &strm) const {
    return strm << "KleeneStar";
}

KleeneStar::KleeneStar(EdgeIndex* index, QueryEdge queryEdge, ResultSorted resultSorted) :
    PhysicalOperator(nullptr, nullptr, resultSorted), queryEdge(queryEdge), index(index) {}
