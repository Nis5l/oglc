# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_SOURCE_DIR = /home/nissl/projects/oglc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nissl/projects/oglc

# Include any dependencies generated for this target.
include lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/flags.make

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.o: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/flags.make
lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.o: lib/glfw-3.3.8/examples/offscreen.c
lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.o: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nissl/projects/oglc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.o"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.o -MF CMakeFiles/offscreen.dir/offscreen.c.o.d -o CMakeFiles/offscreen.dir/offscreen.c.o -c /home/nissl/projects/oglc/lib/glfw-3.3.8/examples/offscreen.c

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/offscreen.dir/offscreen.c.i"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nissl/projects/oglc/lib/glfw-3.3.8/examples/offscreen.c > CMakeFiles/offscreen.dir/offscreen.c.i

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/offscreen.dir/offscreen.c.s"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nissl/projects/oglc/lib/glfw-3.3.8/examples/offscreen.c -o CMakeFiles/offscreen.dir/offscreen.c.s

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/flags.make
lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o: lib/glfw-3.3.8/deps/glad_gl.c
lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nissl/projects/oglc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o -MF CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.d -o CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o -c /home/nissl/projects/oglc/lib/glfw-3.3.8/deps/glad_gl.c

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nissl/projects/oglc/lib/glfw-3.3.8/deps/glad_gl.c > CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nissl/projects/oglc/lib/glfw-3.3.8/deps/glad_gl.c -o CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s

# Object files for target offscreen
offscreen_OBJECTS = \
"CMakeFiles/offscreen.dir/offscreen.c.o" \
"CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o"

# External object files for target offscreen
offscreen_EXTERNAL_OBJECTS =

lib/glfw-3.3.8/examples/offscreen: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/offscreen.c.o
lib/glfw-3.3.8/examples/offscreen: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o
lib/glfw-3.3.8/examples/offscreen: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/build.make
lib/glfw-3.3.8/examples/offscreen: lib/glfw-3.3.8/src/libglfw3.a
lib/glfw-3.3.8/examples/offscreen: /usr/lib64/libm.so
lib/glfw-3.3.8/examples/offscreen: /usr/lib64/librt.a
lib/glfw-3.3.8/examples/offscreen: /usr/lib64/libm.so
lib/glfw-3.3.8/examples/offscreen: /usr/lib64/libX11.so
lib/glfw-3.3.8/examples/offscreen: lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nissl/projects/oglc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable offscreen"
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/offscreen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/build: lib/glfw-3.3.8/examples/offscreen
.PHONY : lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/build

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/clean:
	cd /home/nissl/projects/oglc/lib/glfw-3.3.8/examples && $(CMAKE_COMMAND) -P CMakeFiles/offscreen.dir/cmake_clean.cmake
.PHONY : lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/clean

lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/depend:
	cd /home/nissl/projects/oglc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nissl/projects/oglc /home/nissl/projects/oglc/lib/glfw-3.3.8/examples /home/nissl/projects/oglc /home/nissl/projects/oglc/lib/glfw-3.3.8/examples /home/nissl/projects/oglc/lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glfw-3.3.8/examples/CMakeFiles/offscreen.dir/depend

