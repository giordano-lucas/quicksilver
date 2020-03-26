cmake_minimum_required(VERSION 3.0)
project(quicksilver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
cmake_policy(SET CMP0057 NEW)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-O3")

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
        include/rss.h)


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
        src/rss.c)



file(GLOB_RECURSE STUDENT_HEADER_FILES CONFIGURE_DEPENDS include/student/*.h)
file(GLOB_RECURSE STUDENT_SOURCE_FILES CONFIGURE_DEPENDS src/student/*.cpp)

add_executable(quicksilver src/main.cpp ${SOURCE_FILES} ${HEADER_FILES} ${STUDENT_SOURCE_FILES} ${STUDENT_HEADER_FILES})
target_link_libraries(quicksilver Threads::Threads)

if (Filesystem_FOUND)
    add_executable(benchmarker src/benchmarker.cpp ${SOURCE_FILES} ${HEADER_FILES} ${STUDENT_SOURCE_FILES} ${STUDENT_HEADER_FILES} src/student/myMain.cpp)
    target_link_libraries(benchmarker Threads::Threads std::filesystem)
endif()

