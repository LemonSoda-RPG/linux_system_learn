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
CMAKE_SOURCE_DIR = /home/jiacheng/linux_system_learn/parallel/signal/keybox

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jiacheng/linux_system_learn/parallel/signal/keybox/build

# Include any dependencies generated for this target.
include tbf/CMakeFiles/tbffunctions.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tbf/CMakeFiles/tbffunctions.dir/compiler_depend.make

# Include the progress variables for this target.
include tbf/CMakeFiles/tbffunctions.dir/progress.make

# Include the compile flags for this target's objects.
include tbf/CMakeFiles/tbffunctions.dir/flags.make

tbf/CMakeFiles/tbffunctions.dir/mytbf.c.o: tbf/CMakeFiles/tbffunctions.dir/flags.make
tbf/CMakeFiles/tbffunctions.dir/mytbf.c.o: /home/jiacheng/linux_system_learn/parallel/signal/keybox/tbf/mytbf.c
tbf/CMakeFiles/tbffunctions.dir/mytbf.c.o: tbf/CMakeFiles/tbffunctions.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jiacheng/linux_system_learn/parallel/signal/keybox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tbf/CMakeFiles/tbffunctions.dir/mytbf.c.o"
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT tbf/CMakeFiles/tbffunctions.dir/mytbf.c.o -MF CMakeFiles/tbffunctions.dir/mytbf.c.o.d -o CMakeFiles/tbffunctions.dir/mytbf.c.o -c /home/jiacheng/linux_system_learn/parallel/signal/keybox/tbf/mytbf.c

tbf/CMakeFiles/tbffunctions.dir/mytbf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tbffunctions.dir/mytbf.c.i"
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jiacheng/linux_system_learn/parallel/signal/keybox/tbf/mytbf.c > CMakeFiles/tbffunctions.dir/mytbf.c.i

tbf/CMakeFiles/tbffunctions.dir/mytbf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tbffunctions.dir/mytbf.c.s"
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jiacheng/linux_system_learn/parallel/signal/keybox/tbf/mytbf.c -o CMakeFiles/tbffunctions.dir/mytbf.c.s

# Object files for target tbffunctions
tbffunctions_OBJECTS = \
"CMakeFiles/tbffunctions.dir/mytbf.c.o"

# External object files for target tbffunctions
tbffunctions_EXTERNAL_OBJECTS =

tbf/libtbffunctions.a: tbf/CMakeFiles/tbffunctions.dir/mytbf.c.o
tbf/libtbffunctions.a: tbf/CMakeFiles/tbffunctions.dir/build.make
tbf/libtbffunctions.a: tbf/CMakeFiles/tbffunctions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jiacheng/linux_system_learn/parallel/signal/keybox/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libtbffunctions.a"
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf && $(CMAKE_COMMAND) -P CMakeFiles/tbffunctions.dir/cmake_clean_target.cmake
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tbffunctions.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tbf/CMakeFiles/tbffunctions.dir/build: tbf/libtbffunctions.a
.PHONY : tbf/CMakeFiles/tbffunctions.dir/build

tbf/CMakeFiles/tbffunctions.dir/clean:
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf && $(CMAKE_COMMAND) -P CMakeFiles/tbffunctions.dir/cmake_clean.cmake
.PHONY : tbf/CMakeFiles/tbffunctions.dir/clean

tbf/CMakeFiles/tbffunctions.dir/depend:
	cd /home/jiacheng/linux_system_learn/parallel/signal/keybox/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jiacheng/linux_system_learn/parallel/signal/keybox /home/jiacheng/linux_system_learn/parallel/signal/keybox/tbf /home/jiacheng/linux_system_learn/parallel/signal/keybox/build /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf /home/jiacheng/linux_system_learn/parallel/signal/keybox/build/tbf/CMakeFiles/tbffunctions.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tbf/CMakeFiles/tbffunctions.dir/depend

