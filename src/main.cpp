#include <benches.h>
#include <iostream>
#include <rss.h>
#include <chrono>

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
 * *
 */
 /*
#define count(elem, hashTable, countVar) { \
        if ((hashTable).find(elem) == (hashTable).end()) {(countVar)++; (hashTable).insert({(elem),(elem)});} \
        }
static cardStat eval(IndexIterator it) {
    uint32_t noOut  = 0;
    uint32_t noPath = 0;
    uint32_t noIn   = 0;
    std::unordered_map<uint32_t,uint32_t> hashIn;
    std::unordered_map<uint32_t,uint32_t> hashOut;
    std::unordered_map<Edge,Edge,HashEdge> hashEdge;
    for (;it.hasNext() ; ++it){
        Edge e = *it;
        count(e.target,hashIn, noIn);
        count(e.source,hashOut, noOut);
        count(e,hashEdge, noPath);
    }
    return cardStat{noOut,noPath,noIn};
};
*/
int main(int argc, char *argv[]) {

    if(argc < 3) {
        std::cout << "Usage: quicksilver <graphFile> <queriesFile>" << std::endl;
        argv[1] = "../workload/real/2/graph.nt";
        argv[2] = "../workload/real/2/queries.csv";
        //return 0;
    }

    // args
    std::string graphFile {argv[1]};
    std::string queriesFile {argv[2]};

   // estimatorBench(graphFile, queriesFile);
    auto result = evaluatorBench(graphFile, queriesFile);

    std::cout << std::endl << std::endl << std::endl;
    std::cout << "Total load time: " << result.loadTime << " ms" << std::endl;
    std::cout << "Total prep time: " << result.prepTime << " ms" << std::endl;
    std::cout << "Total eval time: " << result.evalTime << " ms" << std::endl;
    double memoryUsage = double(getPeakRSS()) / 1024.0 / 1024.0;
    std::cout << "Peak memory usage (for all workloads): " << memoryUsage << " MiB" << std::endl;

    return 0;
}
