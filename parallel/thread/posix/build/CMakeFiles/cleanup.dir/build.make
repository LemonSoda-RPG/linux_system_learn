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
include CMakeFiles/cleanup.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cleanup.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cleanup.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cleanup.dir/flags.make

CMakeFiles/cleanup.dir/cleanup.c.o: CMakeFiles/cleanup.dir/flags.make
CMakeFiles/cleanup.dir/cleanup.c.o: /home/jiacheng/linux_system_learn/parallel/thread/posix/cleanup.c
CMakeFiles/cleanup.dir/cleanup.c.o: CMakeFiles/cleanup.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cleanup.dir/cleanup.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cleanup.dir/cleanup.c.o -MF CMakeFiles/cleanup.dir/cleanup.c.o.d -o CMakeFiles/cleanup.dir/cleanup.c.o -c /home/jiacheng/linux_system_learn/parallel/thread/posix/cleanup.c

CMakeFiles/cleanup.dir/cleanup.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/cleanup.dir/cleanup.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jiacheng/linux_system_learn/parallel/thread/posix/cleanup.c > CMakeFiles/cleanup.dir/cleanup.c.i

CMakeFiles/cleanup.dir/cleanup.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/cleanup.dir/cleanup.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jiacheng/linux_system_learn/parallel/thread/posix/cleanup.c -o CMakeFiles/cleanup.dir/cleanup.c.s

# Object files for target cleanup
cleanup_OBJECTS = \
"CMakeFiles/cleanup.dir/cleanup.c.o"

# External object files for target cleanup
cleanup_EXTERNAL_OBJECTS =

cleanup: CMakeFiles/cleanup.dir/cleanup.c.o
cleanup: CMakeFiles/cleanup.dir/build.make
cleanup: CMakeFiles/cleanup.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cleanup"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cleanup.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cleanup.dir/build: cleanup
.PHONY : CMakeFiles/cleanup.dir/build

CMakeFiles/cleanup.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cleanup.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cleanup.dir/clean

CMakeFiles/cleanup.dir/depend:
	cd /home/jiacheng/linux_system_learn/parallel/thread/posix/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jiacheng/linux_system_learn/parallel/thread/posix /home/jiacheng/linux_system_learn/parallel/thread/posix /home/jiacheng/linux_system_learn/parallel/thread/posix/build /home/jiacheng/linux_system_learn/parallel/thread/posix/build /home/jiacheng/linux_system_learn/parallel/thread/posix/build/CMakeFiles/cleanup.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cleanup.dir/depend
