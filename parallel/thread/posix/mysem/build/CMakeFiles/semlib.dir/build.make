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
include CMakeFiles/semlib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/semlib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/semlib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/semlib.dir/flags.make

CMakeFiles/semlib.dir/src/mysem.c.o: CMakeFiles/semlib.dir/flags.make
CMakeFiles/semlib.dir/src/mysem.c.o: /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/src/mysem.c
CMakeFiles/semlib.dir/src/mysem.c.o: CMakeFiles/semlib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/semlib.dir/src/mysem.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/semlib.dir/src/mysem.c.o -MF CMakeFiles/semlib.dir/src/mysem.c.o.d -o CMakeFiles/semlib.dir/src/mysem.c.o -c /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/src/mysem.c

CMakeFiles/semlib.dir/src/mysem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/semlib.dir/src/mysem.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/src/mysem.c > CMakeFiles/semlib.dir/src/mysem.c.i

CMakeFiles/semlib.dir/src/mysem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/semlib.dir/src/mysem.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/src/mysem.c -o CMakeFiles/semlib.dir/src/mysem.c.s

# Object files for target semlib
semlib_OBJECTS = \
"CMakeFiles/semlib.dir/src/mysem.c.o"

# External object files for target semlib
semlib_EXTERNAL_OBJECTS =

libsemlib.a: CMakeFiles/semlib.dir/src/mysem.c.o
libsemlib.a: CMakeFiles/semlib.dir/build.make
libsemlib.a: CMakeFiles/semlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libsemlib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/semlib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/semlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/semlib.dir/build: libsemlib.a
.PHONY : CMakeFiles/semlib.dir/build

CMakeFiles/semlib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/semlib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/semlib.dir/clean

CMakeFiles/semlib.dir/depend:
	cd /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build /home/jiacheng/linux_system_learn/parallel/thread/posix/mysem/build/CMakeFiles/semlib.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/semlib.dir/depend

