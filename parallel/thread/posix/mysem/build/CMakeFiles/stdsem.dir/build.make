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
CMAKE_SOURCE_DIR = /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build

# Include any dependencies generated for this target.
include CMakeFiles/stdsem.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/stdsem.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/stdsem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stdsem.dir/flags.make

CMakeFiles/stdsem.dir/main_sem.c.o: CMakeFiles/stdsem.dir/flags.make
CMakeFiles/stdsem.dir/main_sem.c.o: /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/main_sem.c
CMakeFiles/stdsem.dir/main_sem.c.o: CMakeFiles/stdsem.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/stdsem.dir/main_sem.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/stdsem.dir/main_sem.c.o -MF CMakeFiles/stdsem.dir/main_sem.c.o.d -o CMakeFiles/stdsem.dir/main_sem.c.o -c /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/main_sem.c

CMakeFiles/stdsem.dir/main_sem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/stdsem.dir/main_sem.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/main_sem.c > CMakeFiles/stdsem.dir/main_sem.c.i

CMakeFiles/stdsem.dir/main_sem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/stdsem.dir/main_sem.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/main_sem.c -o CMakeFiles/stdsem.dir/main_sem.c.s

# Object files for target stdsem
stdsem_OBJECTS = \
"CMakeFiles/stdsem.dir/main_sem.c.o"

# External object files for target stdsem
stdsem_EXTERNAL_OBJECTS =

stdsem: CMakeFiles/stdsem.dir/main_sem.c.o
stdsem: CMakeFiles/stdsem.dir/build.make
stdsem: CMakeFiles/stdsem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable stdsem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stdsem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stdsem.dir/build: stdsem
.PHONY : CMakeFiles/stdsem.dir/build

CMakeFiles/stdsem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stdsem.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stdsem.dir/clean

CMakeFiles/stdsem.dir/depend:
	cd /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build/CMakeFiles/stdsem.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/stdsem.dir/depend
