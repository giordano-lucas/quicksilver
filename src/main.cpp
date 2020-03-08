#include <iostream>
#include <chrono>
#include <SimpleGraph.h>
#include <Estimator.h>
#include <SimpleEstimator.h>
#include <SimpleEvaluator.h>
#include <PathQuery.h>
#include "Path.h"
#include "IndexPath.h"
#include "IndexPathGraph.h"
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

int evaluatorBench(std::string &graphFile, std::string &queriesFile) {

    std::cout << "\n(1) Reading the graph into memory and preparing the evaluator...\n" << std::endl;

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

    // prepare the evaluator
    auto est = std::make_shared<SimpleEstimator>(g);
    auto ev = std::make_unique<SimpleEvaluator>(g);

    start = std::chrono::steady_clock::now();
    ev->attachEstimator(est);
    ev->prepare();
    end = std::chrono::steady_clock::now();
    std::cout << "Time to prepare the evaluator: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    std::cout << "\n(2) Running the query workload..." << std::endl;

    auto queries = parseQueries(queriesFile);

    for(auto query : queries) {

        // perform evaluation
        // parse the query into an AST
        std::cout << "\nProcessing query: " << *query;
        std::cout << "Parsed query tree: " << *query->path;

        // perform the evaluation
        start = std::chrono::steady_clock::now();
        auto actual = ev->evaluate(query);
        end = std::chrono::steady_clock::now();

        std::cout << "\nActual (noOut, noPaths, noIn) : ";
        actual.print();
        std::cout << "Time to evaluate: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    }

    for(auto query : queries) delete(query);

    return 0;
}

int main(int argc, char *argv[]) {

    if(argc < 3) {
        std::cout << "Usage: quicksilver <graphFile> <queriesFile>" << std::endl;
        return 0;
    }

    //args
    std::string graphFile {argv[1]};
    std::string queriesFile {argv[2]};

    //estimatorBench(graphFile, queriesFile);
    //evaluatorBench(graphFile, queriesFile);

    Node a = 0;
    Node b = 1;
    Node c = 2;
    Label l1 = 0;
    Label l2 = 1;
    std::vector<Path> paths{
    Path(false, std::vector<Label>{l1}, 2, std::vector<Node>{a,b}),
    Path(false, std::vector<Label>{l2}, 2, std::vector<Node>{b,c}),
    Path(false, std::vector<Label>{l1}, 2, std::vector<Node>{a,c}),
    Path(false, std::vector<Label>{l1}, 2, std::vector<Node>{c,b}),
    Path(false, std::vector<Label>{l1}, 2, std::vector<Node>{b,a}),
    /*Path(false, std::vector<Label>{l2,l1}, 2, std::vector<Node>{a,b,c}),
    Path(false, std::vector<Label>{l1,l2}, 2, std::vector<Node>{a,b,c}),
    Path(false, std::vector<Label>{l1,l2}, 2, std::vector<Node>{a,c,b}),
    Path(false, std::vector<Label>{l1,l2}, 2, std::vector<Node>{a,c,c})*/
    };
    std::cout << "********* SORT ********* \n";
    std::sort(paths.begin(),paths.end());
    for (auto p : paths) std::cout << p;
    std::cout << "****** END SORT ****** \n";
    IndexPath  index;
    index.insertSortedAll(paths);
    std::cout << "********* BUILD K=2 ********* \n";
    //index.buildK2(2);
    index.buildK2MergeJoin(paths,2);
    std::cout << "****** END BUILD K=2 ****** \n";

    IndexResult res = index.getPaths(Path::PrefixPath(std::vector<Label>{l1,l2}, 2, std::vector<Node>{a,c}));
    for (IndexIterator it = res.first; it!= res.second ; ++it){
        std::cout << it->first;
    }
    std::cout << index;
    std::cout << "********* SORT SOURCE********* \n";
    std::sort(paths.begin(),paths.end(), sourceFirstComp);
    for (auto p : paths) std::cout << p;
    std::cout << "****** END SORT SOURCE ****** \n";
    std::cout << "********* SORT TARGET********* \n";
    std::sort(paths.begin(),paths.end(), targetFirstComp);
    for (auto p : paths) std::cout << p;
    std::cout << "****** END SORT TARGET ****** \n";

    /*Try to compute the index */
    IndexPathGraph indexGraph;

    auto start = std::chrono::steady_clock::now();
    try {
        indexGraph.readFromContiguousFile(graphFile);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 0;
    }
    std::cout << "EGDES = " << indexGraph.getNoEdges();

    return 0;
}



