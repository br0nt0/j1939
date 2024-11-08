# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/br0nt0/workspace/j1939

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/br0nt0/workspace/j1939/build/utest

# Utility rule file for coverage.

# Include any custom commands dependencies for this target.
include tests/CMakeFiles/coverage.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/coverage.dir/progress.make

tests/CMakeFiles/coverage: tests/j1939_uTests
tests/CMakeFiles/coverage: tests/run_gcovr_parser

tests/run_gcovr_parser:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/br0nt0/workspace/j1939/build/utest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running gcovr to produce XML and HTML coverage reports"
	gcovr --root /home/br0nt0/workspace/j1939 --txt-metric branch --html-details --output /home/br0nt0/workspace/j1939/build/utest/coverage.html --cobertura /home/br0nt0/workspace/j1939/build/utest/coverage.xml --cobertura-pretty

coverage: tests/CMakeFiles/coverage
coverage: tests/run_gcovr_parser
coverage: tests/CMakeFiles/coverage.dir/build.make
	cd /home/br0nt0/workspace/j1939/build/utest/tests && firefox /home/br0nt0/workspace/j1939/build/utest/coverage.html
.PHONY : coverage

# Rule to build all files generated by this target.
tests/CMakeFiles/coverage.dir/build: coverage
.PHONY : tests/CMakeFiles/coverage.dir/build

tests/CMakeFiles/coverage.dir/clean:
	cd /home/br0nt0/workspace/j1939/build/utest/tests && $(CMAKE_COMMAND) -P CMakeFiles/coverage.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/coverage.dir/clean

tests/CMakeFiles/coverage.dir/depend:
	cd /home/br0nt0/workspace/j1939/build/utest && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/br0nt0/workspace/j1939 /home/br0nt0/workspace/j1939/tests /home/br0nt0/workspace/j1939/build/utest /home/br0nt0/workspace/j1939/build/utest/tests /home/br0nt0/workspace/j1939/build/utest/tests/CMakeFiles/coverage.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/coverage.dir/depend

