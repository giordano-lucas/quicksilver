cmake_minimum_required(VERSION 3.0)
project(quicksilver)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#set(CMAKE_CXX_FLAGS "-O3")

include_directories(include)

set(HEADER_FILES
        include/PathTree.h
        include/Graph.h
        include/Evaluator.h
        include/Estimator.h
        include/SimpleGraph.h
        include/SimpleEstimator.h
        include/SimpleEvaluator.h
        include/Path.h include/IndexPath.h include/IndexPathGraph.h)

set(SOURCE_FILES
        src/main.cpp
        src/PathTree.cpp
        src/SimpleGraph.cpp
        src/SimpleEstimator.cpp
        src/SimpleEvaluator.cpp
        src/PathQuery.cpp
        include/Path.h src/Path.cpp include/IndexPath.h src/IndexPath.cpp include/IndexPathGraph.h src/IndexPathGraph.cpp)

add_executable(quicksilver ${SOURCE_FILES} ${HEADER_FILES})
