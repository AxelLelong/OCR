# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jules_/Desktop/Epita/OCR/OCR

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/OCR.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/OCR.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/OCR.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OCR.dir/flags.make

CMakeFiles/OCR.dir/XOR/Maths.c.o: CMakeFiles/OCR.dir/flags.make
CMakeFiles/OCR.dir/XOR/Maths.c.o: ../XOR/Maths.c
CMakeFiles/OCR.dir/XOR/Maths.c.o: CMakeFiles/OCR.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/OCR.dir/XOR/Maths.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/OCR.dir/XOR/Maths.c.o -MF CMakeFiles/OCR.dir/XOR/Maths.c.o.d -o CMakeFiles/OCR.dir/XOR/Maths.c.o -c /Users/jules_/Desktop/Epita/OCR/OCR/XOR/Maths.c

CMakeFiles/OCR.dir/XOR/Maths.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/OCR.dir/XOR/Maths.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jules_/Desktop/Epita/OCR/OCR/XOR/Maths.c > CMakeFiles/OCR.dir/XOR/Maths.c.i

CMakeFiles/OCR.dir/XOR/Maths.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/OCR.dir/XOR/Maths.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jules_/Desktop/Epita/OCR/OCR/XOR/Maths.c -o CMakeFiles/OCR.dir/XOR/Maths.c.s

CMakeFiles/OCR.dir/XOR/reseau_test.c.o: CMakeFiles/OCR.dir/flags.make
CMakeFiles/OCR.dir/XOR/reseau_test.c.o: ../XOR/reseau_test.c
CMakeFiles/OCR.dir/XOR/reseau_test.c.o: CMakeFiles/OCR.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/OCR.dir/XOR/reseau_test.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/OCR.dir/XOR/reseau_test.c.o -MF CMakeFiles/OCR.dir/XOR/reseau_test.c.o.d -o CMakeFiles/OCR.dir/XOR/reseau_test.c.o -c /Users/jules_/Desktop/Epita/OCR/OCR/XOR/reseau_test.c

CMakeFiles/OCR.dir/XOR/reseau_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/OCR.dir/XOR/reseau_test.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/jules_/Desktop/Epita/OCR/OCR/XOR/reseau_test.c > CMakeFiles/OCR.dir/XOR/reseau_test.c.i

CMakeFiles/OCR.dir/XOR/reseau_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/OCR.dir/XOR/reseau_test.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/jules_/Desktop/Epita/OCR/OCR/XOR/reseau_test.c -o CMakeFiles/OCR.dir/XOR/reseau_test.c.s

# Object files for target OCR
OCR_OBJECTS = \
"CMakeFiles/OCR.dir/XOR/Maths.c.o" \
"CMakeFiles/OCR.dir/XOR/reseau_test.c.o"

# External object files for target OCR
OCR_EXTERNAL_OBJECTS =

OCR: CMakeFiles/OCR.dir/XOR/Maths.c.o
OCR: CMakeFiles/OCR.dir/XOR/reseau_test.c.o
OCR: CMakeFiles/OCR.dir/build.make
OCR: CMakeFiles/OCR.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable OCR"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OCR.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OCR.dir/build: OCR
.PHONY : CMakeFiles/OCR.dir/build

CMakeFiles/OCR.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OCR.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OCR.dir/clean

CMakeFiles/OCR.dir/depend:
	cd /Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jules_/Desktop/Epita/OCR/OCR /Users/jules_/Desktop/Epita/OCR/OCR /Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug /Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug /Users/jules_/Desktop/Epita/OCR/OCR/cmake-build-debug/CMakeFiles/OCR.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OCR.dir/depend

