# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build

# Include any dependencies generated for this target.
include CMakeFiles/3.1.3_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/3.1.3_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/3.1.3_test.dir/flags.make

CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.o: CMakeFiles/3.1.3_test.dir/flags.make
CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.o: ../tests/test_data_types.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.o   -c /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/tests/test_data_types.c

CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/tests/test_data_types.c > CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.i

CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/tests/test_data_types.c -o CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.s

# Object files for target 3.1.3_test
3_1_3_test_OBJECTS = \
"CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.o"

# External object files for target 3.1.3_test
3_1_3_test_EXTERNAL_OBJECTS =

lib3.1.3_test.so: CMakeFiles/3.1.3_test.dir/tests/test_data_types.c.o
lib3.1.3_test.so: CMakeFiles/3.1.3_test.dir/build.make
lib3.1.3_test.so: CMakeFiles/3.1.3_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library lib3.1.3_test.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3.1.3_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/3.1.3_test.dir/build: lib3.1.3_test.so

.PHONY : CMakeFiles/3.1.3_test.dir/build

CMakeFiles/3.1.3_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/3.1.3_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/3.1.3_test.dir/clean

CMakeFiles/3.1.3_test.dir/depend:
	cd /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3 /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3 /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build /root/csd/devon-e-budzitowski-basic/exercises/C/3.1.3/build/CMakeFiles/3.1.3_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/3.1.3_test.dir/depend

