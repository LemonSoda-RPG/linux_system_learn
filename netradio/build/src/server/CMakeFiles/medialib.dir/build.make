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
CMAKE_SOURCE_DIR = /home/jc/linux_c/netradio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jc/linux_c/netradio/build

# Include any dependencies generated for this target.
include src/server/CMakeFiles/medialib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/server/CMakeFiles/medialib.dir/compiler_depend.make

# Include the progress variables for this target.
include src/server/CMakeFiles/medialib.dir/progress.make

# Include the compile flags for this target's objects.
include src/server/CMakeFiles/medialib.dir/flags.make

src/server/CMakeFiles/medialib.dir/medialib.c.o: src/server/CMakeFiles/medialib.dir/flags.make
src/server/CMakeFiles/medialib.dir/medialib.c.o: /home/jc/linux_c/netradio/src/server/medialib.c
src/server/CMakeFiles/medialib.dir/medialib.c.o: src/server/CMakeFiles/medialib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jc/linux_c/netradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/server/CMakeFiles/medialib.dir/medialib.c.o"
	cd /home/jc/linux_c/netradio/build/src/server && /usr/sbin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/server/CMakeFiles/medialib.dir/medialib.c.o -MF CMakeFiles/medialib.dir/medialib.c.o.d -o CMakeFiles/medialib.dir/medialib.c.o -c /home/jc/linux_c/netradio/src/server/medialib.c

src/server/CMakeFiles/medialib.dir/medialib.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/medialib.dir/medialib.c.i"
	cd /home/jc/linux_c/netradio/build/src/server && /usr/sbin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jc/linux_c/netradio/src/server/medialib.c > CMakeFiles/medialib.dir/medialib.c.i

src/server/CMakeFiles/medialib.dir/medialib.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/medialib.dir/medialib.c.s"
	cd /home/jc/linux_c/netradio/build/src/server && /usr/sbin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jc/linux_c/netradio/src/server/medialib.c -o CMakeFiles/medialib.dir/medialib.c.s

# Object files for target medialib
medialib_OBJECTS = \
"CMakeFiles/medialib.dir/medialib.c.o"

# External object files for target medialib
medialib_EXTERNAL_OBJECTS =

src/server/libmedialib.a: src/server/CMakeFiles/medialib.dir/medialib.c.o
src/server/libmedialib.a: src/server/CMakeFiles/medialib.dir/build.make
src/server/libmedialib.a: src/server/CMakeFiles/medialib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jc/linux_c/netradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libmedialib.a"
	cd /home/jc/linux_c/netradio/build/src/server && $(CMAKE_COMMAND) -P CMakeFiles/medialib.dir/cmake_clean_target.cmake
	cd /home/jc/linux_c/netradio/build/src/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/medialib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/server/CMakeFiles/medialib.dir/build: src/server/libmedialib.a
.PHONY : src/server/CMakeFiles/medialib.dir/build

src/server/CMakeFiles/medialib.dir/clean:
	cd /home/jc/linux_c/netradio/build/src/server && $(CMAKE_COMMAND) -P CMakeFiles/medialib.dir/cmake_clean.cmake
.PHONY : src/server/CMakeFiles/medialib.dir/clean

src/server/CMakeFiles/medialib.dir/depend:
	cd /home/jc/linux_c/netradio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jc/linux_c/netradio /home/jc/linux_c/netradio/src/server /home/jc/linux_c/netradio/build /home/jc/linux_c/netradio/build/src/server /home/jc/linux_c/netradio/build/src/server/CMakeFiles/medialib.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/server/CMakeFiles/medialib.dir/depend
