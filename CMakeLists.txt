cmake_minimum_required(VERSION 3.0)
project(quicksilver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
cmake_policy(SET CMP0057 NEW)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_C_FLAGS "-O0")
set(CMAKE_CXX_FLAGS "-O0")

include_directories(include)
include_directories(include/student)

find_package(Threads REQUIRED)
find_package(Filesystem)

set(HEADER_FILES
        include/PathTree.h
        include/Graph.h
        include/Evaluator.h
        include/Estimator.h
        include/SimpleGraph.h
        include/SimpleEstimator.h
        include/SimpleEvaluator.h
        include/student/Edge.h src/student/Edge.cpp
        include/student/PhysicalOperator.h
        src/student/IndexLookUp.cpp include/student/IndexLookUp.h
        src/student/MergeJoin.cpp include/student/MergeJoin.h
        include/PathQuery.h
        include/benches.h
        include/rss.h include/student/Synopse.h src/student/HashJoin.cpp include/student/HashJoin.h src/student/Planner.cpp include/student/Planner.h include/student/Subtree.h src/student/IndexJoin.cpp include/student/IndexJoin.h src/student/IndexLookUp2.cpp include/student/IndexLookUp2.h include/student/AdjGraph.cpp include/student/AdjGraph.h)


set(SOURCE_FILES
        src/PathTree.cpp
        src/SimpleGraph.cpp
        src/SimpleEstimator.cpp
        src/SimpleEvaluator.cpp
        src/PathQuery.cpp
        include/student/Edge.h src/student/Edge.cpp
        include/student/PhysicalOperator.h
        src/student/IndexLookUp.cpp include/student/IndexLookUp.h
        include/student/BlockingQueue.h
        src/student/MergeJoin.cpp include/student/MergeJoin.h
        src/student/KleeneStar.cpp include/student/KleeneStar.h
        src/benches.cpp
        src/rss.c include/student/Synopse.h src/student/HashJoin.cpp include/student/HashJoin.h src/student/Planner.cpp include/student/Planner.h include/student/Subtree.h src/student/IndexJoin.cpp include/student/IndexJoin.h src/student/IndexLookUp2.cpp include/student/IndexLookUp2.h include/student/AdjGraph.cpp include/student/AdjGraph.h)



file(GLOB_RECURSE STUDENT_HEADER_FILES CONFIGURE_DEPENDS include/student/*.h)
file(GLOB_RECURSE STUDENT_SOURCE_FILES CONFIGURE_DEPENDS src/student/*.cpp)

add_executable(quicksilver src/main.cpp ${SOURCE_FILES} ${HEADER_FILES} ${STUDENT_SOURCE_FILES} ${STUDENT_HEADER_FILES} include/student/Synopse.h src/student/HashJoin.cpp include/student/HashJoin.h src/student/Planner.cpp include/student/Planner.h include/student/Subtree.h src/student/IndexJoin.cpp include/student/IndexJoin.h src/student/IndexLookUp2.cpp include/student/IndexLookUp2.h include/student/AdjGraph.cpp include/student/AdjGraph.h)
target_link_libraries(quicksilver Threads::Threads)

if (Filesystem_FOUND)
    add_executable(benchmarker src/benchmarker.cpp ${SOURCE_FILES} ${HEADER_FILES} ${STUDENT_SOURCE_FILES} ${STUDENT_HEADER_FILES} src/student/myMain.cpp include/student/HelperPhysicalOperator.h src/student/HelperPhysicalOperator.cpp include/student/HelperPhysicalOperator.h include/student/Synopse.h src/student/HashJoin.cpp include/student/HashJoin.h src/student/Planner.cpp include/student/Planner.h include/student/Subtree.h src/student/IndexJoin.cpp include/student/IndexJoin.h src/student/IndexLookUp2.cpp include/student/IndexLookUp2.h include/student/AdjGraph.cpp include/student/AdjGraph.h)
    target_link_libraries(benchmarker Threads::Threads std::filesystem)
endif()

