# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/valeriyaprokopova/dev/databases/quicksilver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/benchmarker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/benchmarker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/benchmarker.dir/flags.make

CMakeFiles/benchmarker.dir/src/benchmarker.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/benchmarker.cpp.o: ../src/benchmarker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/benchmarker.dir/src/benchmarker.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/benchmarker.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/benchmarker.cpp

CMakeFiles/benchmarker.dir/src/benchmarker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/benchmarker.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/benchmarker.cpp > CMakeFiles/benchmarker.dir/src/benchmarker.cpp.i

CMakeFiles/benchmarker.dir/src/benchmarker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/benchmarker.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/benchmarker.cpp -o CMakeFiles/benchmarker.dir/src/benchmarker.cpp.s

CMakeFiles/benchmarker.dir/src/PathTree.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/PathTree.cpp.o: ../src/PathTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/benchmarker.dir/src/PathTree.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/PathTree.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/PathTree.cpp

CMakeFiles/benchmarker.dir/src/PathTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/PathTree.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/PathTree.cpp > CMakeFiles/benchmarker.dir/src/PathTree.cpp.i

CMakeFiles/benchmarker.dir/src/PathTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/PathTree.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/PathTree.cpp -o CMakeFiles/benchmarker.dir/src/PathTree.cpp.s

CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.o: ../src/SimpleGraph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleGraph.cpp

CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleGraph.cpp > CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.i

CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleGraph.cpp -o CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.s

CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.o: ../src/SimpleEstimator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleEstimator.cpp

CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleEstimator.cpp > CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.i

CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleEstimator.cpp -o CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.s

CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.o: ../src/SimpleEvaluator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleEvaluator.cpp

CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleEvaluator.cpp > CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.i

CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/SimpleEvaluator.cpp -o CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.s

CMakeFiles/benchmarker.dir/src/PathQuery.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/PathQuery.cpp.o: ../src/PathQuery.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/benchmarker.dir/src/PathQuery.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/PathQuery.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/PathQuery.cpp

CMakeFiles/benchmarker.dir/src/PathQuery.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/PathQuery.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/PathQuery.cpp > CMakeFiles/benchmarker.dir/src/PathQuery.cpp.i

CMakeFiles/benchmarker.dir/src/PathQuery.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/PathQuery.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/PathQuery.cpp -o CMakeFiles/benchmarker.dir/src/PathQuery.cpp.s

CMakeFiles/benchmarker.dir/src/student/Edge.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/Edge.cpp.o: ../src/student/Edge.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/benchmarker.dir/src/student/Edge.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/Edge.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/Edge.cpp

CMakeFiles/benchmarker.dir/src/student/Edge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/Edge.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/Edge.cpp > CMakeFiles/benchmarker.dir/src/student/Edge.cpp.i

CMakeFiles/benchmarker.dir/src/student/Edge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/Edge.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/Edge.cpp -o CMakeFiles/benchmarker.dir/src/student/Edge.cpp.s

CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.o: ../src/student/IndexLookUp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/IndexLookUp.cpp

CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/IndexLookUp.cpp > CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.i

CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/IndexLookUp.cpp -o CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.s

CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.o: ../src/student/MergeJoin.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/MergeJoin.cpp

CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/MergeJoin.cpp > CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.i

CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/MergeJoin.cpp -o CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.s

CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.o: ../src/student/KleeneStar.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/KleeneStar.cpp

CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/KleeneStar.cpp > CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.i

CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/KleeneStar.cpp -o CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.s

CMakeFiles/benchmarker.dir/src/benches.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/benches.cpp.o: ../src/benches.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/benchmarker.dir/src/benches.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/benches.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/benches.cpp

CMakeFiles/benchmarker.dir/src/benches.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/benches.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/benches.cpp > CMakeFiles/benchmarker.dir/src/benches.cpp.i

CMakeFiles/benchmarker.dir/src/benches.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/benches.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/benches.cpp -o CMakeFiles/benchmarker.dir/src/benches.cpp.s

CMakeFiles/benchmarker.dir/src/rss.c.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/rss.c.o: ../src/rss.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/benchmarker.dir/src/rss.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/benchmarker.dir/src/rss.c.o   -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/rss.c

CMakeFiles/benchmarker.dir/src/rss.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/benchmarker.dir/src/rss.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/rss.c > CMakeFiles/benchmarker.dir/src/rss.c.i

CMakeFiles/benchmarker.dir/src/rss.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/benchmarker.dir/src/rss.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/rss.c -o CMakeFiles/benchmarker.dir/src/rss.c.s

CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.o: ../src/student/HashJoin.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/HashJoin.cpp

CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/HashJoin.cpp > CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.i

CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/HashJoin.cpp -o CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.s

CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.o: ../src/student/HelperPhysicalOperator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/HelperPhysicalOperator.cpp

CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/HelperPhysicalOperator.cpp > CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.i

CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/HelperPhysicalOperator.cpp -o CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.s

CMakeFiles/benchmarker.dir/src/student/Planner.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/Planner.cpp.o: ../src/student/Planner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/benchmarker.dir/src/student/Planner.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/Planner.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/Planner.cpp

CMakeFiles/benchmarker.dir/src/student/Planner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/Planner.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/Planner.cpp > CMakeFiles/benchmarker.dir/src/student/Planner.cpp.i

CMakeFiles/benchmarker.dir/src/student/Planner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/Planner.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/Planner.cpp -o CMakeFiles/benchmarker.dir/src/student/Planner.cpp.s

CMakeFiles/benchmarker.dir/src/student/myMain.cpp.o: CMakeFiles/benchmarker.dir/flags.make
CMakeFiles/benchmarker.dir/src/student/myMain.cpp.o: ../src/student/myMain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/benchmarker.dir/src/student/myMain.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmarker.dir/src/student/myMain.cpp.o -c /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/myMain.cpp

CMakeFiles/benchmarker.dir/src/student/myMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmarker.dir/src/student/myMain.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/myMain.cpp > CMakeFiles/benchmarker.dir/src/student/myMain.cpp.i

CMakeFiles/benchmarker.dir/src/student/myMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmarker.dir/src/student/myMain.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/valeriyaprokopova/dev/databases/quicksilver/src/student/myMain.cpp -o CMakeFiles/benchmarker.dir/src/student/myMain.cpp.s

# Object files for target benchmarker
benchmarker_OBJECTS = \
"CMakeFiles/benchmarker.dir/src/benchmarker.cpp.o" \
"CMakeFiles/benchmarker.dir/src/PathTree.cpp.o" \
"CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.o" \
"CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.o" \
"CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.o" \
"CMakeFiles/benchmarker.dir/src/PathQuery.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/Edge.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.o" \
"CMakeFiles/benchmarker.dir/src/benches.cpp.o" \
"CMakeFiles/benchmarker.dir/src/rss.c.o" \
"CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/Planner.cpp.o" \
"CMakeFiles/benchmarker.dir/src/student/myMain.cpp.o"

# External object files for target benchmarker
benchmarker_EXTERNAL_OBJECTS =

benchmarker: CMakeFiles/benchmarker.dir/src/benchmarker.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/PathTree.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/SimpleGraph.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/SimpleEstimator.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/SimpleEvaluator.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/PathQuery.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/Edge.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/IndexLookUp.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/MergeJoin.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/KleeneStar.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/benches.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/rss.c.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/HashJoin.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/HelperPhysicalOperator.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/Planner.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/src/student/myMain.cpp.o
benchmarker: CMakeFiles/benchmarker.dir/build.make
benchmarker: CMakeFiles/benchmarker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking CXX executable benchmarker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/benchmarker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/benchmarker.dir/build: benchmarker

.PHONY : CMakeFiles/benchmarker.dir/build

CMakeFiles/benchmarker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/benchmarker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/benchmarker.dir/clean

CMakeFiles/benchmarker.dir/depend:
	cd /Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/valeriyaprokopova/dev/databases/quicksilver /Users/valeriyaprokopova/dev/databases/quicksilver /Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug /Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug /Users/valeriyaprokopova/dev/databases/quicksilver/cmake-build-debug/CMakeFiles/benchmarker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/benchmarker.dir/depend

