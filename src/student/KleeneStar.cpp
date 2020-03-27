//
// Created by Lucas Giordano on 24/03/2020.
//


#include "KleeneStar.h"


KleeneStar::KleeneStar(std::shared_ptr<SimpleGraph>& index, QueryEdge queryEdge) :
        PhysicalOperator(nullptr, nullptr, TARGET_SORTED), queryEdge(queryEdge), index(index) {
    if (queryEdge.source != NONE) query.push_back(basic_query_t{selection, queryEdge.source});
    query.push_back(basic_query_t{kleene, queryEdge.label});
    if (queryEdge.target != NONE) query.push_back(basic_query_t{selection, queryEdge.target});
}

uint32_t KleeneStar::cost() const {
    return getCardinality().noPaths;
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
#define getTarget(it) ((!reversed)?(*it).target:(*it).source)
void KleeneStar::dfs(std::unordered_map<Node,bool>& discovered,Node n, Node source, bool reversed,std::vector<Edge>& output){
    //use cache if possible
    /*if (finishedCache.find(n) != finishedCache.end()){
        std::vector<Node> cacheRes = cache.find(n)->second;
        for (auto target : cacheRes) output.push_back((reversed)?Edge{target,source}:Edge{source,target});
    }*/
    //else explore the graph
    if (output.size() > 0) discovered.insert({n,true});//set vertex n to discovered  /!\ in the first iteration output.size == 0 => we do not set (source) to be discovered yet (otherwise we won't allow (source,source) to be a part of the result)
    auto it = (!reversed)?index->getEdgesSource(QueryEdge{n,queryEdge.label,NONE}):index->getEdgesTarget(QueryEdge{NONE,queryEdge.label,n});
    for (; it.hasNext(); ++it){     //explore all nodes in base.adj[n]
       Node target =(*it).target;
        if (discovered.find(target) == discovered.end()){    //if target has not been explored yet => explore
            output.push_back((reversed)?Edge{target,source}:Edge{source,target});//(source,target) is part of the results of the physical operator
            dfs(discovered,target,source,reversed,output);       //explore
        }
    }
    finishedCache.insert({n, true});
}
/**
 * Kleene star based on DFS to allow the projection operation to be pushed down to the leaves
 * Complexity :
 *          - O((E+V) * V) if queryEdge  = (*,l,*)
 *          - O(E + V)     if queryEdge  = (n,l,*) or (*,l,n)
 */
void KleeneStar::evalPipeline(ResultSorted resultSorted) {
     //get data from the database
    std::unordered_map<Node,bool> discovered;
    std::vector<Edge> output;
    if      (queryEdge.source != NONE) dfs(discovered,queryEdge.source,queryEdge.source, false,output);
    else if (queryEdge.target != NONE) dfs(discovered,queryEdge.target,queryEdge.target, true,output); //in reverse adj list => need to re-reverse the edges at the end
    for (auto e : output) {
        out.push(e,false);
        if (terminated) return;       //stop computation if parent operator has what it wants
    }
    if (queryEdge.source == NONE && queryEdge.target == NONE){
        //if the query is not bounded (*,l,*) => perform dfs for all sources nodes
        IndexIterator it;
        switch(resultSorted){
            case NOT_SORTED:   //use source sorted by default
            case SOURCE_SORTED:  it = index->getEdgesSource(QueryEdge{NONE,queryEdge.label,NONE}); break;
            case TARGET_SORTED:  it = index->getEdgesTarget(QueryEdge{NONE,queryEdge.label,NONE},false);break;
        }
        for (;it.hasNext() && !terminated; it.skip()){
            dfs(discovered,(*it).source,(*it).source,(resultSorted == TARGET_SORTED)?true:false,output);
            std::sort(output.begin(),output.end(),(resultSorted == TARGET_SORTED)?targetComp:sourceComp);
            for (auto e : output) {
                out.push(e,false);
                if (terminated) return;       //stop computation if parent operator has what it wants
            }
            discovered.clear(); //reset for next computation
            output.clear();
        }
    }
    out.push(END_EDGE,true);
    done = true;
}

Edge KleeneStar::produceNextEdge() {
    if (!done || out.size() > 0)  return out.pop();
    else                          return END_EDGE;
}

std::ostream &KleeneStar::name(std::ostream &strm) const {
    return strm << "KleeneStar";
}


/*void evalPipeline2() {
    IndexResult res = index->getEdgesSource(QueryEdge{NONE,queryEdge.label,NONE}); //get data from the database
    std::unordered_multimap<Node,Node> base;  //graph adj list representation
    std::map<Node,bool> setNodes;             //set of nodes sorted
    std::unordered_map<Node,bool> discovered;
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

    if      (queryEdge.source != NONE) dfs(base,discovered,queryEdge.source,queryEdge.source, false);
    else if (queryEdge.target != NONE) dfs(base,discovered,queryEdge.target,queryEdge.target, true); //in reverse adj list => need to re-reverse the edges at the end
    else {  //if the query is not bounded (*,l,*) => perform dfs for all sources nodes
        for (auto it = setNodes.begin(); it != setNodes.end(); ++it) {
           // std::cout << "Kleene : " << it->first << "\n";
            Node key = it->first;
            discovered.clear();
            dfs(base,discovered,key,key,false);

        }
    }
    out.push(END_EDGE,true);
}
*/