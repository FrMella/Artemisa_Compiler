# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/francisco/Desktop/clion-2019.1.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/francisco/Desktop/clion-2019.1.3/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/francisco/CLionProjects/Artemisa

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/francisco/CLionProjects/Artemisa/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Artemisa.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Artemisa.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Artemisa.dir/flags.make

CMakeFiles/Artemisa.dir/main.c.o: CMakeFiles/Artemisa.dir/flags.make
CMakeFiles/Artemisa.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/francisco/CLionProjects/Artemisa/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Artemisa.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Artemisa.dir/main.c.o   -c /home/francisco/CLionProjects/Artemisa/main.c

CMakeFiles/Artemisa.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Artemisa.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/francisco/CLionProjects/Artemisa/main.c > CMakeFiles/Artemisa.dir/main.c.i

CMakeFiles/Artemisa.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Artemisa.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/francisco/CLionProjects/Artemisa/main.c -o CMakeFiles/Artemisa.dir/main.c.s

CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.o: CMakeFiles/Artemisa.dir/flags.make
CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.o: ../Core/inlineASM_thumb.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/francisco/CLionProjects/Artemisa/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.o   -c /home/francisco/CLionProjects/Artemisa/Core/inlineASM_thumb.c

CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/francisco/CLionProjects/Artemisa/Core/inlineASM_thumb.c > CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.i

CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/francisco/CLionProjects/Artemisa/Core/inlineASM_thumb.c -o CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.s

# Object files for target Artemisa
Artemisa_OBJECTS = \
"CMakeFiles/Artemisa.dir/main.c.o" \
"CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.o"

# External object files for target Artemisa
Artemisa_EXTERNAL_OBJECTS =

Artemisa: CMakeFiles/Artemisa.dir/main.c.o
Artemisa: CMakeFiles/Artemisa.dir/Core/inlineASM_thumb.c.o
Artemisa: CMakeFiles/Artemisa.dir/build.make
Artemisa: CMakeFiles/Artemisa.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/francisco/CLionProjects/Artemisa/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Artemisa"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Artemisa.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Artemisa.dir/build: Artemisa

.PHONY : CMakeFiles/Artemisa.dir/build

CMakeFiles/Artemisa.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Artemisa.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Artemisa.dir/clean

CMakeFiles/Artemisa.dir/depend:
	cd /home/francisco/CLionProjects/Artemisa/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/francisco/CLionProjects/Artemisa /home/francisco/CLionProjects/Artemisa /home/francisco/CLionProjects/Artemisa/cmake-build-debug /home/francisco/CLionProjects/Artemisa/cmake-build-debug /home/francisco/CLionProjects/Artemisa/cmake-build-debug/CMakeFiles/Artemisa.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Artemisa.dir/depend
