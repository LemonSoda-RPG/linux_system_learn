# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/jiacheng/linux_system_learn/parallel/thread/posix

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jiacheng/linux_system_learn/parallel/thread/posix/build

# Include any dependencies generated for this target.
include CMakeFiles/primer0_poolre.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/primer0_poolre.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/primer0_poolre.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/primer0_poolre.dir/flags.make

CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o: CMakeFiles/primer0_poolre.dir/flags.make
CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o: /home/jiacheng/linux_system_learn/parallel/thread/posix/primer0_poolre.c
CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o: CMakeFiles/primer0_poolre.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o -MF CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o.d -o CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o -c /home/jiacheng/linux_system_learn/parallel/thread/posix/primer0_poolre.c

CMakeFiles/primer0_poolre.dir/primer0_poolre.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/primer0_poolre.dir/primer0_poolre.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jiacheng/linux_system_learn/parallel/thread/posix/primer0_poolre.c > CMakeFiles/primer0_poolre.dir/primer0_poolre.c.i

CMakeFiles/primer0_poolre.dir/primer0_poolre.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/primer0_poolre.dir/primer0_poolre.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jiacheng/linux_system_learn/parallel/thread/posix/primer0_poolre.c -o CMakeFiles/primer0_poolre.dir/primer0_poolre.c.s

# Object files for target primer0_poolre
primer0_poolre_OBJECTS = \
"CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o"

# External object files for target primer0_poolre
primer0_poolre_EXTERNAL_OBJECTS =

primer0_poolre: CMakeFiles/primer0_poolre.dir/primer0_poolre.c.o
primer0_poolre: CMakeFiles/primer0_poolre.dir/build.make
primer0_poolre: CMakeFiles/primer0_poolre.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable primer0_poolre"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/primer0_poolre.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/primer0_poolre.dir/build: primer0_poolre
.PHONY : CMakeFiles/primer0_poolre.dir/build

CMakeFiles/primer0_poolre.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/primer0_poolre.dir/cmake_clean.cmake
.PHONY : CMakeFiles/primer0_poolre.dir/clean

CMakeFiles/primer0_poolre.dir/depend:
	cd /home/jiacheng/linux_system_learn/parallel/thread/posix/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jiacheng/linux_system_learn/parallel/thread/posix /home/jiacheng/linux_system_learn/parallel/thread/posix /home/jiacheng/linux_system_learn/parallel/thread/posix/build /home/jiacheng/linux_system_learn/parallel/thread/posix/build /home/jiacheng/linux_system_learn/parallel/thread/posix/build/CMakeFiles/primer0_poolre.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/primer0_poolre.dir/depend

