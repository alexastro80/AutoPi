# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/AutoPi/AutoPi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/AutoPi/AutoPi

# Utility rule file for AutoPi_autogen.

# Include the progress variables for this target.
include CMakeFiles/AutoPi_autogen.dir/progress.make

CMakeFiles/AutoPi_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/pi/AutoPi/AutoPi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target AutoPi"
	/usr/bin/cmake -E cmake_autogen /home/pi/AutoPi/AutoPi/CMakeFiles/AutoPi_autogen.dir/AutogenInfo.cmake Release

AutoPi_autogen: CMakeFiles/AutoPi_autogen
AutoPi_autogen: CMakeFiles/AutoPi_autogen.dir/build.make

.PHONY : AutoPi_autogen

# Rule to build all files generated by this target.
CMakeFiles/AutoPi_autogen.dir/build: AutoPi_autogen

.PHONY : CMakeFiles/AutoPi_autogen.dir/build

CMakeFiles/AutoPi_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AutoPi_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AutoPi_autogen.dir/clean

CMakeFiles/AutoPi_autogen.dir/depend:
	cd /home/pi/AutoPi/AutoPi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/AutoPi/AutoPi /home/pi/AutoPi/AutoPi /home/pi/AutoPi/AutoPi /home/pi/AutoPi/AutoPi /home/pi/AutoPi/AutoPi/CMakeFiles/AutoPi_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AutoPi_autogen.dir/depend

