# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/algorithms_from_the_scratch.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/algorithms_from_the_scratch.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/algorithms_from_the_scratch.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/algorithms_from_the_scratch.dir/flags.make

CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o: CMakeFiles/algorithms_from_the_scratch.dir/flags.make
CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o: /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/7-greedy/4-brute-force.cpp
CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o: CMakeFiles/algorithms_from_the_scratch.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o -MF CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o.d -o CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o -c /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/7-greedy/4-brute-force.cpp

CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/7-greedy/4-brute-force.cpp > CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.i

CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/7-greedy/4-brute-force.cpp -o CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.s

# Object files for target algorithms_from_the_scratch
algorithms_from_the_scratch_OBJECTS = \
"CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o"

# External object files for target algorithms_from_the_scratch
algorithms_from_the_scratch_EXTERNAL_OBJECTS =

algorithms_from_the_scratch: CMakeFiles/algorithms_from_the_scratch.dir/7-greedy/4-brute-force.cpp.o
algorithms_from_the_scratch: CMakeFiles/algorithms_from_the_scratch.dir/build.make
algorithms_from_the_scratch: CMakeFiles/algorithms_from_the_scratch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable algorithms_from_the_scratch"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/algorithms_from_the_scratch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/algorithms_from_the_scratch.dir/build: algorithms_from_the_scratch
.PHONY : CMakeFiles/algorithms_from_the_scratch.dir/build

CMakeFiles/algorithms_from_the_scratch.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/algorithms_from_the_scratch.dir/cmake_clean.cmake
.PHONY : CMakeFiles/algorithms_from_the_scratch.dir/clean

CMakeFiles/algorithms_from_the_scratch.dir/depend:
	cd /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug /Users/hliangzhao/Documents/GitHub/algorithms-from-the-scratch/cmake-build-debug/CMakeFiles/algorithms_from_the_scratch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/algorithms_from_the_scratch.dir/depend

