//
// Created by Lucas Giordano on 26/03/2020.
//
#include <benches.h>
#include <iostream>
#include <chrono>
#include <SimpleGraph.h>
#include <Estimator.h>
#include <SimpleEstimator.h>
#include <SimpleEvaluator.h>
#include <PathQuery.h>
#include "Edge.h"
#include "IndexLookUp.h"
#include "PhysicalOperator.h"
#include <thread>
#include <KleeneStar.h>
#include "BlockingQueue.h"
#include "MergeJoin.h"
#include <rss.h>
/*
int myEvaluatorBench(std::string &graphFile, std::string &queriesFile);

std::vector<PathQuery *> parseQueries(std::string &fileName) {

    std::vector<PathQuery *> queries {};

    std::string line;
    std::ifstream graphFile { fileName };

    std::regex edgePat (R"((.+),(.+),(.+))");

    while(std::getline(graphFile, line)) {
        std::smatch matches;

        // match edge data
        if(std::regex_search(line, matches, edgePat)) {
            std::string s = matches[1];
            auto pt = PathTree::strToTree(matches[2]);
            std::string t = matches[3];

            queries.emplace_back(new PathQuery(s, pt, t));
        }
    }

    graphFile.close();

    if(queries.size() == 0) std::cout << "Did not parse any queries... Check query file." << std::endl;

    return queries;
}

int estimatorBench(std::string &graphFile, std::string &queriesFile) {

    std::cout << "\n(1) Reading the graph into memory and preparing the estimator...\n" << std::endl;

    // read the graph
    auto g = std::make_shared<SimpleGraph>();

    auto start = std::chrono::steady_clock::now();
    try {
        g->readFromContiguousFile(graphFile);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time to read the graph into memory: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    // prepare the estimator
    auto est = std::make_unique<SimpleEstimator>(g);
    start = std::chrono::steady_clock::now();
    est->prepare();
    end = std::chrono::steady_clock::now();
    std::cout << "Time to prepare the estimator: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    std::cout << "\n(2) Running the query workload..." << std::endl;

    auto queries = parseQueries(queriesFile);

    for(auto query : queries) {

        // perform estimation
        // parse the query into an AST
        std::cout << "\nProcessing query: " << *query;
        std::cout << "Parsed query tree: " << *query->path;

        start = std::chrono::steady_clock::now();
        auto estimate = est->estimate(query);
        end = std::chrono::steady_clock::now();

        std::cout << "\nEstimation (noOut, noPaths, noIn) : ";
        estimate.print();
        std::cout << "Time to estimate: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

        // perform evaluation
        auto ev = std::make_unique<SimpleEvaluator>(g);
        ev->prepare();
        start = std::chrono::steady_clock::now();
        auto actual = ev->evaluate(query);
        end = std::chrono::steady_clock::now();

        std::cout << "Actual (noOut, noPaths, noIn) : ";
        actual.print();
        std::cout << "Time to evaluate: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    }

    for(auto query : queries) delete(query);

    return 0;
}




int main(int argc, char *argv[]) {

    if(argc < 3) {
        std::cout << "Usage: quicksilver <graphFile> <queriesFile>" << std::endl;
        argv[1] = "../testfiles/workload/real/1/graph.nt";
        argv[2] = "../testfiles/workload/real/1/queries.csv";
        //return 0;
    }



    //args
    std::string graphFile {argv[1]};
    std::string queriesFile {argv[2]};

    //estimatorBench(graphFile, queriesFile);
    //evaluatorBench(graphFile, queriesFile);
    //std::cout << "============================================================================================ \n";
    myEvaluatorBench(graphFile,queriesFile);

    Label l1 = 0;
    Label l2 = 1;
    std::vector<Edge> edges{
        Edge{0,l1,0},
        Edge{1,l1,0},
        Edge{1,l1,1},
        Edge{0,l1,1},
        Edge{4,l1,4},

        Edge{0,l2,4},
        Edge{0,l2,7},
        Edge{3,l2,0},
        Edge{4,l2,0},
        Edge{5,l2,5}
    };

    EdgeIndex  index;
    index.insertAll(edges);

    //PhysicalOperator* op = new MergeJoin(new IndexLookUp(&index,Edge{NONE,l1,NONE},false, TARGET_SORTED),new IndexLookUp(&index,Edge{NONE,l2,NONE},false,SOURCE_SORTED));
    PhysicalOperator* op = new KleeneStar(&index, Edge{NONE,l1,NONE},NOT_SORTED);
    op->eval().print();

    // ********************************************************************************
    //Try to compute the index

    EdgeIndex index;

    try {
        index.buildFromFile(graphFile);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }

    PhysicalOperator* op = new MergeJoin(new IndexLookUp(&index,Edge{NONE,0,NONE},false, TARGET_SORTED),new IndexLookUp(&index,Edge{NONE,1,NONE},false,SOURCE_SORTED));
    //PhysicalOperator* op = new IndexLookUp(&index, Edge{NONE,0,NONE},false, TARGET_SORTED);
    op->eval().print();

    //Construct physical indexLookUp operator
    Edge query = Edge{13,3,NONE};
    IndexLookUp op(&index,query, true,SOURCE_SORTED);

    auto start = std::chrono::steady_clock::now();
    cardStat r=op.eval();
    auto end = std::chrono::steady_clock::now();
    r.print();
    std::cout << "TIME TO EVALUATE INDEX : " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;



    //From upstream repository
//    auto result = evaluatorBench(graphFile, queriesFile);
//
//    std::cout << std::endl << std::endl << std::endl;
//    std::cout << "Total load time: " << result.loadTime << " ms" << std::endl;
//    std::cout << "Total prep time: " << result.prepTime << " ms" << std::endl;
//    std::cout << "Total eval time: " << result.evalTime << " ms" << std::endl;
//    double memoryUsage = double(getPeakRSS()) / 1024.0 / 1024.0;
//    std::cout << "Peak memory usage (for all workloads): " << memoryUsage << " MiB" << std::endl;


    return 0;
}


static PhysicalOperator* ofPathTree(PathTree* tree, std::shared_ptr<SimpleGraph> & index, Node leftBounded, Node rightBounded){
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
static PhysicalOperator* ofPathQuery(PathQuery* pq, std::shared_ptr<SimpleGraph> & index) {
    Node s = (pq->s.compare("*")==0)?NONE: (uint32_t) std::stoi(pq->s);
    Node t = (pq->t.compare("*")==0)?NONE: (uint32_t) std::stoi(pq->t);
    //std::cout << "(" << s << "," << t<< ")\n";
    return ofPathTree(pq->path, index,s,t);
}

int compareEvaluatorBench(std::string &graphFile, std::string &queriesFile) {

    std::cout << "\n(1) Reading the graph into memory and preparing the evaluator...\n" << std::endl;

    // read the graph
    EdgeIndex index;
    auto start = std::chrono::steady_clock::now();
    try {
        index.readFromContiguousFile(graphFile);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    auto end = std::chrono::steady_clock::now();




    std::cout << "Time to read the graph into memory: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;




    start = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
    std::cout << "Time to prepare the evaluator: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    std::cout << "\n(2) Running the query workload..." << std::endl;

    auto queries = parseQueries(queriesFile);

    for(auto query : queries) {

        // perform evaluation
        // parse the query into an AST
        std::cout << "\nProcessing query: " << *query;
        std::cout << "Parsed query tree: " << *query->path;
        PhysicalOperator* evaluator = ofPathQuery(query, &index);
        //std::cout << *evaluator;

        // perform the evaluation
        start = std::chrono::steady_clock::now();
        cardStat actual = evaluator->eval();
        end = std::chrono::steady_clock::now();

        std::cout << "\nActual (noOut, noPaths, noIn) : ";
        actual.print();
        std::cout << "Time to evaluate: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
        delete evaluator;
    }

    for(auto query : queries) delete(query);

    return 0;
}

int myEvaluatorBench(std::string &graphFile, std::string &queriesFile) {

    std::cout << "\n(1) Reading the graph into memory and preparing the evaluator...\n" << std::endl;

    // read the graph
    EdgeIndex index;
    auto start = std::chrono::steady_clock::now();
    try {
        index.readFromContiguousFile(graphFile);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    auto end = std::chrono::steady_clock::now();
    // read adj list
    auto g = std::make_shared<SimpleGraph>();

    auto start2 = std::chrono::steady_clock::now();
    try {
        g->readFromContiguousFile(graphFile);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    auto end2 = std::chrono::steady_clock::now();

    std::cout << "Time dif to read the graph into memory: " << std::chrono::duration<double, std::milli>(end2 - start2 - (end - start)).count() << " ms" << std::endl;

    // ***************** ESTIMATION CONSTRUCTION ******************

    start = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();



    // prepare the evaluator ADJ
    auto est = std::make_shared<SimpleEstimator>(g);
    auto ev = std::make_unique<SimpleEvaluator>(g);

    start2 = std::chrono::steady_clock::now();
    ev->attachEstimator(est);
    ev->prepare();
    end2 = std::chrono::steady_clock::now();
    std::cout << "Time dif to prepare the evaluator: " << std::chrono::duration<double, std::milli>(end2 - start2 - (end - start)).count() << " ms" << std::endl;


    std::cout << "\n(2) Running the query workload..." << std::endl;

    auto queries = parseQueries(queriesFile);

    for(auto query : queries) {

        // perform evaluation
        // parse the query into an AST
        std::cout << "\nProcessing query: " << *query;
        std::cout << "Parsed query tree: " << *query->path;
        PhysicalOperator* evaluator = ofPathQuery(query, g);
        //std::cout << *evaluator;

        // perform the evaluation
        start = std::chrono::steady_clock::now();
        cardStat actual = evaluator->eval();
        end = std::chrono::steady_clock::now();

        // perform the evaluation ADJ
        start2 = std::chrono::steady_clock::now();
        auto actual2 = ev->evaluate(query);
        end2 = std::chrono::steady_clock::now();

        std::cout << "\nActual (noOut, noPaths, noIn) : ";
        cardStat dif = cardStat{actual.noOut - actual2.noOut, actual.noPaths - actual2.noPaths, actual.noIn - actual2.noIn};
        dif.print();
        std::cout << "Time dif to evaluate: " << std::chrono::duration<double, std::milli>(end2 - start2 - (end - start)).count() << " ms" << std::endl;

        delete evaluator;
    }

    for(auto query : queries) delete(query);

    return 0;
}*/
