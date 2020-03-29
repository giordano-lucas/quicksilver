//
// Created by Valeriya Prokopova on 23/03/2020.
//
#include <IndexLookUp.h>
#include "MergeJoin.h"
#include "Planner.h"
#include <unordered_map>
#include <KleeneStar.h>
#include "Edge.h"
#include "Subtree.h"
#include "HelperPhysicalOperator.h"
#include "PathTree.h"


//std::vector<std::string>
void traverseInOrder(PathTree *pt, std::vector<std::string> &flattenedTree) {
    if (pt->isLeaf()) {
        flattenedTree.push_back(pt->data);
        return;
    }
    /* first recurse on left child */
    traverseInOrder(pt->left, flattenedTree);
    /* now recurse on right child */
    traverseInOrder(pt->right, flattenedTree);
}

std::vector<std::string> flattenedTree(PathTree *pt) {
    std::vector<std::string> flattenedTree;
    traverseInOrder(pt, flattenedTree);
    return flattenedTree;
}

// Returns subset of consecutive children (the order must be preserved) of size n
std::vector<Subtree> subtreesOfSize(std::vector<std::string> children, uint32_t n) {
    //std::vector<std::vector<std::string>> subsetsOfSizeN;
    std::vector<Subtree> subtreesOfSizeN;
    for (uint32_t i = 0; i <= children.size() - n; i++) {
        std::vector<std::string> subtree;
        for (uint32_t j = i; j <= i + n - 1; j++) {
            subtree.push_back(children.at(j));
        }
        subtreesOfSizeN.push_back(Subtree{subtree, i, i + n - 1});
    }
    return subtreesOfSizeN;
}

bool areNeighbors(Subtree subtreeLeft, Subtree subtreeRight) {
    //check if subtrees are neighbours.
    //Note, the order of these subtrees matter (left, then right)
    return (subtreeLeft.e_index == subtreeRight.s_index - 1);
}

Subtree mergeSubtrees(Subtree leftSubtree, Subtree rightSubtree) {
    std::vector<std::string> mergedSubtree;
    // add all children from left subtree:
    for (uint32_t i = 0; i <= leftSubtree.subtree.size() - 1; i++) {
        mergedSubtree.push_back(leftSubtree.subtree[i]);
    }
    // add all children from right subtree:
    for (uint32_t i = 0; i <= rightSubtree.subtree.size() - 1; i++) {
        mergedSubtree.push_back(rightSubtree.subtree[i]);
    }
    Subtree subtree = {mergedSubtree, leftSubtree.s_index, rightSubtree.e_index};
    return subtree;
}

PhysicalOperator *Planner::generatePlanForUnboundedQuery(PathQuery *query, std::shared_ptr<SimpleGraph> &index,
                                                         std::shared_ptr<SimpleEstimator> &e) {
    //Create PathTree from query
    PathTree *tree = query->path;
    std::string s = query->s;
    std::string t = query->t;

    //Children contains all children of the root of the flattened pathTree
    std::vector<std::string> children = flattenedTree(tree);
    Subtree fullSubtree = {children, 0, (uint32_t) children.size() - 1};


    //Map vector of children (aka logical plan) to its best physical plan
    std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash> planMap;
    std::vector<int> costOfMerged;

    //Get all subtrees of size 1
    std::vector<Subtree> subtreesOfSizeOne = subtreesOfSize(children, 1);
    //Create physical plan for all subtrees of size 1
    for (uint32_t i = 0; i < subtreesOfSizeOne.size(); i++) {
        Subtree subtree = subtreesOfSizeOne[i];
        std::string str_subtree = subtree.subtree[0];

        Node leftBounded = NONE;
        Node rightBounded = NONE;
        // Pushing selection operation as far as possible:
        // if 's' is bounded, then we should pass the value of 's' only to the the leftmost child of the tree.
        // 'NONE' is passed to the rest of the children.
        if (i == 0 && !(s == "*")) {
            leftBounded = (uint32_t) std::stoi(s);
        }
        // Pushing selection operation as far as possible:
        // if 't' is bounded, then we should pass the value of 't' only to the the rightmost child of the tree.
        // 'NONE' is passed to the rest of the children.
        if (i == subtreesOfSizeOne.size() - 1 && !(t == "*")) {
            rightBounded = (uint32_t) std::stoi(t);
        }
        PhysicalOperator *physicalPlan;

        std::regex directLabel(R"((\d+)\>)");
        std::regex inverseLabel(R"((\d+)\<)");
        std::regex kleeneStar(R"((\d+)\+)");

        std::smatch matches;
        uint32_t label;

        bool inverse;
        if (std::regex_search(str_subtree, matches, directLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            physicalPlan = new IndexLookUp(index, QueryEdge{leftBounded, label, rightBounded}, false);
        } else if (std::regex_search(str_subtree, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            physicalPlan = new IndexLookUp(index, QueryEdge{leftBounded, label, rightBounded}, true);
        } else if (std::regex_search(str_subtree, matches, kleeneStar)) {
            label = (uint32_t) std::stoul(matches[1]);
            physicalPlan = new KleeneStar(index, QueryEdge{leftBounded, label, rightBounded});
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
        }

        // Add subtree --> physical plan to hashmap:
        planMap.insert({subtree, physicalPlan});
    };

//    //testing:
//    printChildren(children);
//    printContentsOfHash(planMap);
//    testPlanHash(index);
    std::cout << children.size() << std::endl;
    // Increasingly calculate the best physical plan (PhysicalOperator) for each subtree of a given size
    for (uint32_t size = 2; size <= children.size(); size++) {
        // Divide the subtree size into left and right subtrees
        for (uint32_t leftSize = 1; leftSize <= size - 1; leftSize++) {
            uint32_t rightSize = size - leftSize;

            std::vector<Subtree> leftSizeSubtrees = subtreesOfSize(children, leftSize);
            for (Subtree leftSizeSubtree: leftSizeSubtrees) {

                std::vector<Subtree> rightSizeSubtrees = subtreesOfSize(children, rightSize);
                for (Subtree rightSizeSubtree: rightSizeSubtrees) {

                    if (areNeighbors(leftSizeSubtree, rightSizeSubtree)) {

                        std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash>::const_iterator leftPlanIter = planMap.find(
                                leftSizeSubtree);
                        std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash>::const_iterator rightPlanIter = planMap.find(
                                rightSizeSubtree);
                        // merge two subTrees:
                        Subtree mergedSubtree = mergeSubtrees(leftSizeSubtree, rightSizeSubtree);
                        // printChildren(mergedSubtree.subtree);

                        std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash>::const_iterator mergedPlanIter = planMap.find(
                                mergedSubtree);

                        // At this point, we know that second element of left and right plans must exist because
                        // this algo increasingly calculates the best plan starting from the smallest-size subtree up.
                        PhysicalOperator *physicalOperatorMergeJoin = new MergeJoin(leftPlanIter->second,
                                                                                    rightPlanIter->second);
                        physicalOperatorMergeJoin->attachEstimator(e);
                        uint32_t costNewPlan;
                        costNewPlan = physicalOperatorMergeJoin->cost();

                        // Check if a physical plan for the merged tree already exists.
                        if (mergedPlanIter == planMap.end()) {
                            // Physical plan for this merged tree does not exist yet.
                            //std::cout << "Plan for merged subtree not found" << std::endl;
                            // Add subtree --> physical plan to hashmap:
                            planMap.insert({mergedSubtree, physicalOperatorMergeJoin});
                            if (mergedSubtree.subtree.size() == 3) {
                                costOfMerged.push_back(costNewPlan);
                            }

                        } else {
                            // Physical plan for the merged tree exists.
                            // Check if the cost of the existing plan is greater than the cost of the new plan.
                            std::cout << "Physical plan for the merged tree exists." << std::endl;
                            //retrieve the old PhysicalOperator

                            uint32_t costOldPlan = mergedPlanIter->second->cost();
                            if (costNewPlan < costOldPlan) {
                                std::cout << "Old plan is worse" << std::endl;
                                // update to a new Physical Plan (because it has smaller cost)
                                planMap[mergedSubtree] = physicalOperatorMergeJoin;
                                if (mergedSubtree.subtree.size() == 3) {
                                    costOfMerged.push_back(costNewPlan);
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    // The final result (best Physical Plan for a given query) should now be stored in planMap.
    std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash>::const_iterator fullMapIter = planMap.find(
            fullSubtree);
    if (fullMapIter == planMap.end()) {
        std::cout << "Final query result is not stored in a hash map. SOMETHING WENT WRONG!!!" << std::endl;
    }
    PhysicalOperator *result = fullMapIter->second;
//    PhysicalOperator resultingPlan = *result;
    std::cout << "Final hash map" << std::endl;
    printContentsOfHash(planMap);
    std::cout << "\n Starting to delete contents of hash map\n";
//    deleteContentsOfHash(&planMap, fullSubtree);
    std::cout << "\ncontents of hash map deleted\n";
    printContentsOfHash(planMap);
    return result;
}


void printChildren(std::vector<std::string> children) {
    std::cout << "" << std::endl;
    std::cout << "Children: ";
    for (std::vector<std::string>::const_iterator i = children.begin(); i != children.end(); ++i) {
        std::cout << *i << ' ';
        std::cout << " ";
    }
    std::cout << "\n";
}

PathTree *generateRightDeepTree(std::vector<std::string> children) {
    printChildren(children);
    std::string concat = "/";
    if (children.size() == 2) {
        PathTree *leftLeaf = new PathTree(children[0], nullptr, nullptr);
        PathTree *rightLeaf = new PathTree(children[1], nullptr, nullptr);
        PathTree *finalTree = new PathTree(concat, leftLeaf, rightLeaf);
        return finalTree;
    }
    std::string str = children[0];
    PathTree *leftLeaf = new PathTree(str, nullptr, nullptr);
    children.erase(children.begin(), children.begin() + 1);
    PathTree *resultTree = new PathTree(concat, leftLeaf, generateRightDeepTree(children));
    return resultTree;
}


PhysicalOperator *
Planner::generatePlan(PathQuery *query, std::shared_ptr<SimpleGraph> &index, std::shared_ptr<SimpleEstimator> &e) {
    //Create PathTree from query
    PathTree *tree = query->path;
    std::string s = query->s;
    std::string t = query->t;
    std::string regExp = tree->data;

    bool optimizationEnabled = true;

    if (optimizationEnabled) {

        PhysicalOperator *op;
        if (s != "*") {
            op = ofPathQuery(query, index);
            op->attachEstimator(e);
        } else if (t != "*") {
            //Children contains all children of the root of the flattened pathTree
            std::vector<std::string> children = flattenedTree(tree);
            std::vector<std::string> children_right_deep = flattenedTree(tree);
            PathTree *rightDeepTree = generateRightDeepTree(children_right_deep);

//            if(rightDeepTree->left == nullptr && rightDeepTree->right == nullptr) {
//                std::cout << ' ' << rightDeepTree->data << ' ';
//            } else {
//                std::cout << '(' << rightDeepTree->data << ' ';
//                if(rightDeepTree->left != nullptr) std::cout << *rightDeepTree->left;
//                if(rightDeepTree->right!= nullptr) std::cout << *rightDeepTree->right;
//                std::cout << ')';
//            }

            op = ofPathTree(rightDeepTree, index, NONE, std::stoi(t));
            op->attachEstimator(e);
            printChildren(children_right_deep);
            printChildren(children);
        } else {
            op = generatePlanForUnboundedQuery(query, index, e);
        }
        return op;
    } else {
        return generatePlanForUnboundedQuery(query, index, e);
    }
}


void Planner::testPlanHash(std::shared_ptr<SimpleGraph> &index) {

    std::vector<std::vector<std::string>> children_vector;
    children_vector = {{"1<", "2+"},
                       {"1<", "2>"},
                       {"1+", "2>", "2<", "10<"},
                       {"3<"},
                       {"1+", "2>", "2<", "10<", "133+"}};
    std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash> planMap;

    // insert each children vector to plan Hash
    std::cout << "TESTING HASH FUNCTION HERE: " << std::endl;
    for (uint32_t i = 0; i < children_vector.size(); i++) {
        Subtree subtree = {children_vector[i]};
        PhysicalOperator *test = new IndexLookUp(index, QueryEdge{10, 1, NONE}, false);
        planMap.insert({subtree, test});
    }

    // print the contents of the hash map
    printContentsOfHash(planMap);

    // try finding elements in the hash map
    std::cout << "NOW TRY TO FIND GIVEN ELEMENTS:" << std::endl;
    Subtree input;
    input.subtree = {"1<", "2+"};
    input.subtree = {"3++"};
    std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash>::const_iterator got = planMap.find(input);

    if (got == planMap.end()) {
        std::cout << "not found";
    } else {
        std::cout << "{ ";
        for (uint32_t i = 0; i <= got->first.subtree.size() - 1; i++) {
            std::cout << got->first.subtree[i] << " ";
        }
        std::cout << "} is " << got->second;
    }
    std::cout << std::endl;
}



//    std::vector<int> costOfMerged;
void Planner::printCosts(std::vector<int> costs) {
    std::cout << "" << std::endl;
    std::cout << "Costs of Final Result: ";
    for (std::vector<int>::const_iterator i = costs.begin(); i != costs.end(); ++i) {
        std::cout << *i << ' ';
        std::cout << " ";
    }
    std::cout << "\n";
}

void Planner::printContentsOfHash(std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash> planMap) {
    std::cout << "" << std::endl;
    for (auto const &pair: planMap) {
        std::vector<std::string> children = pair.first.subtree;
        uint32_t s_i = pair.first.s_index;
        uint32_t e_i = pair.first.e_index;
        std::cout << "{ ";
        for (uint32_t i = 0; i <= children.size() - 1; i++) {
            std::cout << children[i] << " ";
        }
        std::cout << "}, start/end indices: (" << s_i << "," << e_i << ") --> " << pair.second << std::endl;
    }
}

void Planner::deleteContentsOfHash(std::unordered_map<Subtree, PhysicalOperator *, SubtreeHash> *planMap, Subtree fullSubtree) {
    for (auto const &pair: *planMap) {
        uint32_t keyStartIndex = pair.first.s_index;
        uint32_t keyEndIndex = pair.first.e_index;
        uint32_t fullSubtreeStartIndex = fullSubtree.s_index;
        uint32_t fullSubtreeEndIndex = fullSubtree.e_index;
        if (!(keyStartIndex == fullSubtreeStartIndex && keyEndIndex == fullSubtreeEndIndex)) {
            planMap->erase(pair.first);
        }
    }
}

