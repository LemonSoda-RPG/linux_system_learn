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
CMAKE_SOURCE_DIR = /home/jc/linux_c/parallel/signal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jc/linux_c/build

# Include any dependencies generated for this target.
include CMakeFiles/block.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/block.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/block.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/block.dir/flags.make

CMakeFiles/block.dir/block.c.o: CMakeFiles/block.dir/flags.make
CMakeFiles/block.dir/block.c.o: /home/jc/linux_c/parallel/signal/block.c
CMakeFiles/block.dir/block.c.o: CMakeFiles/block.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jc/linux_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/block.dir/block.c.o"
	/usr/sbin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/block.dir/block.c.o -MF CMakeFiles/block.dir/block.c.o.d -o CMakeFiles/block.dir/block.c.o -c /home/jc/linux_c/parallel/signal/block.c

CMakeFiles/block.dir/block.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/block.dir/block.c.i"
	/usr/sbin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jc/linux_c/parallel/signal/block.c > CMakeFiles/block.dir/block.c.i

CMakeFiles/block.dir/block.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/block.dir/block.c.s"
	/usr/sbin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jc/linux_c/parallel/signal/block.c -o CMakeFiles/block.dir/block.c.s

# Object files for target block
block_OBJECTS = \
"CMakeFiles/block.dir/block.c.o"

# External object files for target block
block_EXTERNAL_OBJECTS =

block: CMakeFiles/block.dir/block.c.o
block: CMakeFiles/block.dir/build.make
block: CMakeFiles/block.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jc/linux_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable block"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/block.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/block.dir/build: block
.PHONY : CMakeFiles/block.dir/build

CMakeFiles/block.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/block.dir/cmake_clean.cmake
.PHONY : CMakeFiles/block.dir/clean

CMakeFiles/block.dir/depend:
	cd /home/jc/linux_c/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jc/linux_c/parallel/signal /home/jc/linux_c/parallel/signal /home/jc/linux_c/build /home/jc/linux_c/build /home/jc/linux_c/build/CMakeFiles/block.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/block.dir/depend

