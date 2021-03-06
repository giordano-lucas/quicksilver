//
// Created by Lucas Giordano on 24/03/2020.
//


#include "KleeneStar.h"


KleeneStar::KleeneStar(std::shared_ptr<SimpleGraph>& index, QueryEdge queryEdge) :
        PhysicalOperator(nullptr, nullptr, SOURCE_SORTED,TC), queryEdge(queryEdge), index(index) {
    if (queryEdge.source != NONE) query.push_back(basic_query_t{selection, queryEdge.source});
    query.push_back(basic_query_t{kleene, queryEdge.label});
    if (queryEdge.target != NONE) query.push_back(basic_query_t{selection, queryEdge.target});
}

uint32_t KleeneStar::cost() {
    return getCardinality().noPaths;
}


/**
 * ====================> NOT WORKING YET <============================
 */

void KleeneStar::cacheDfs(std::unordered_map<Node,bool>& discovered,Node n ,bool reversed, bool firstIt){
    if (!firstIt) discovered.insert({n,true});//set vertex n to discovered
    //use cache if possible
    if (finishedCache.find(n) != finishedCache.end()) return; //nothing to explore if this the last node is in cache
    //else explore the graph
    auto it = (!reversed)?index->getEdgesSource(QueryEdge{n,queryEdge.label,NONE}):index->getEdgesTarget(QueryEdge{NONE,queryEdge.label,n});
    std::vector<Node> targets;
    for (; it.hasNext(); ++it){     //explore all nodes in base.adj[n]
        Node target =(*it).target;
        targets.push_back(target);
        if (discovered.find(target) == discovered.end()){    //if target has not been explored yet => explore
            cacheDfs(discovered,target,reversed,false);       //explore
        }
    }
    std::vector<Node> reachable;
    for (Node t : targets){
        auto cacheRes = cache.find(t);
        if (cacheRes == cache.end()) continue;
        //assert(cacheRes != cache.end());
        reachable.push_back(t);
        reachable.insert( reachable.end(), cacheRes->second.begin(), cacheRes->second.end());
    }
    cache.insert({n,reachable});
    finishedCache.insert({n, true});
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
    //auto it = (reversed)?index->sourcesReachable(queryEdge.label,n):index->targetsReachable(queryEdge.label,n);
    for (; it.hasNext(); ++it){     //explore all nodes in base.adj[n]
       Node target =(*it).target;
        if (discovered.find(target) == discovered.end()){    //if target has not been explored yet => explore
            output.push_back((reversed)?Edge{target,source}:Edge{source,target});//(source,target) is part of the results of the physical operator
            dfs(discovered,target,source,reversed,output);       //explore
        }
    }
}
typedef std::vector<Edge> &Data ;
/*void crank(Data c, Data delta, Index* index){
    uint32_t noPaths = 0;
//    IndexIterator l  = index->getSour
    while(r.hasNext() && l.hasNext()){
        while (l.hasNext() && ((*l).target < (*r).source)) {++l;} //advance until we have a joining edge
        Edge tl = *l;
        uint32_t nbLeft = 0;
        bool done = false;
        while  (!done && l.hasNext()){ //setL contains all edges in Left that have the same target value
            if (tl.target == (*l).target){nbLeft++;++l;}
            else done = true;
        }
        while (r.hasNext() && (*r).source < tl.target) {++r;} //advance until we have a joining edge
        if    (r.hasNext() && tl.target == (*r).source)  middleMap.insert({tl.target,true}); //have a middle
        while (r.hasNext() && tl.target == (*r).source){
            twoMap.insert({*r,true}); //have a two
            inMap.insert({(*r).target,true});  //have an in
            noPaths += nbLeft;
            ++r;
        }
    }
}*/

void KleeneStar::pushEdges(std::vector<Edge> &edges, bool reversed){
    Edge last = END_EDGE;
    for (Edge e : edges){ //do not send duplicate edges
        if (e != last) out.push((reversed)?reverse(e):e,false);
        last = e;
        if (terminated) return; //stop computation if parent operator has what it wants
    }
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
    pushEdges(output,false); //push edges in out
    if (queryEdge.source == NONE && queryEdge.target == NONE){
        //if the query is not bounded (*,l,*) => perform dfs for all sources nodes
        IndexIterator it;
        switch(resultSorted){
            case NOT_SORTED:   //use source sorted by default
            case SOURCE_SORTED:  it = index->getEdgesSource(QueryEdge{NONE,queryEdge.label,NONE}); break;
            case TARGET_SORTED:  it = index->getEdgesTarget(QueryEdge{NONE,queryEdge.label,NONE});break;
        }
        for (;it.hasNext() && !terminated; it.skip()){
            Node s = (*it).source;
            discovered.clear(); //reset for next computation
            output.clear();
           /* ///// TEST CACHE DFS
            bool reversed = (resultSorted == TARGET_SORTED)?true:false;
            cacheDfs(discovered,s,reversed, true);
            auto cacheRes = cache[s];
            std::sort(cacheRes.begin(),cacheRes.end());
            Edge last = END_EDGE;
            for (Node t : cacheRes){ //do not send duplicate edges
                Edge e = (reversed)?Edge{t,s}:Edge{s,t};
                if (e != last) out.push(e,false);
                last = e;
                if (terminated) return; //stop computation if parent operator has what it wants
            }*/
            dfs(discovered,(*it).source,(*it).source,(resultSorted == TARGET_SORTED)?true:false,output);
            std::sort(output.begin(),output.end(),(resultSorted == TARGET_SORTED)?targetComp:sourceComp);
            pushEdges(output,false);

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

void KleeneStar::skip(Node until) {
    return;
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