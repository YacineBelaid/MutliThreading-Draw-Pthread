# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/devone/Bureau/inf5171-tp1-233.1-Source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/devone/Bureau/inf5171-tp1-233.1-Source/build

# Include any dependencies generated for this target.
include src/CMakeFiles/fractale_benchmark.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/fractale_benchmark.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/fractale_benchmark.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/fractale_benchmark.dir/flags.make

src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o: src/CMakeFiles/fractale_benchmark.dir/flags.make
src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o: /home/devone/Bureau/inf5171-tp1-233.1-Source/src/fractale_benchmark.cpp
src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o: src/CMakeFiles/fractale_benchmark.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/devone/Bureau/inf5171-tp1-233.1-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o"
	cd /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o -MF CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o.d -o CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o -c /home/devone/Bureau/inf5171-tp1-233.1-Source/src/fractale_benchmark.cpp

src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.i"
	cd /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/devone/Bureau/inf5171-tp1-233.1-Source/src/fractale_benchmark.cpp > CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.i

src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.s"
	cd /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/devone/Bureau/inf5171-tp1-233.1-Source/src/fractale_benchmark.cpp -o CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.s

# Object files for target fractale_benchmark
fractale_benchmark_OBJECTS = \
"CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o"

# External object files for target fractale_benchmark
fractale_benchmark_EXTERNAL_OBJECTS =

bin/fractale_benchmark: src/CMakeFiles/fractale_benchmark.dir/fractale_benchmark.cpp.o
bin/fractale_benchmark: src/CMakeFiles/fractale_benchmark.dir/build.make
bin/fractale_benchmark: lib/libfractalelib.a
bin/fractale_benchmark: src/CMakeFiles/fractale_benchmark.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/devone/Bureau/inf5171-tp1-233.1-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/fractale_benchmark"
	cd /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fractale_benchmark.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/fractale_benchmark.dir/build: bin/fractale_benchmark
.PHONY : src/CMakeFiles/fractale_benchmark.dir/build

src/CMakeFiles/fractale_benchmark.dir/clean:
	cd /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src && $(CMAKE_COMMAND) -P CMakeFiles/fractale_benchmark.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/fractale_benchmark.dir/clean

src/CMakeFiles/fractale_benchmark.dir/depend:
	cd /home/devone/Bureau/inf5171-tp1-233.1-Source/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/devone/Bureau/inf5171-tp1-233.1-Source /home/devone/Bureau/inf5171-tp1-233.1-Source/src /home/devone/Bureau/inf5171-tp1-233.1-Source/build /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src /home/devone/Bureau/inf5171-tp1-233.1-Source/build/src/CMakeFiles/fractale_benchmark.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/fractale_benchmark.dir/depend
