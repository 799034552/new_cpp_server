# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/string/c/c/new_cpp_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/string/c/c/new_cpp_server/build

# Include any dependencies generated for this target.
include cpp_server/CMakeFiles/cpp_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include cpp_server/CMakeFiles/cpp_server.dir/compiler_depend.make

# Include the progress variables for this target.
include cpp_server/CMakeFiles/cpp_server.dir/progress.make

# Include the compile flags for this target's objects.
include cpp_server/CMakeFiles/cpp_server.dir/flags.make

cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o: cpp_server/CMakeFiles/cpp_server.dir/flags.make
cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o: ../cpp_server/src/Client/Client.cpp
cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o: cpp_server/CMakeFiles/cpp_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/string/c/c/new_cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o -MF CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o.d -o CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o -c /home/string/c/c/new_cpp_server/cpp_server/src/Client/Client.cpp

cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpp_server.dir/src/Client/Client.cpp.i"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/string/c/c/new_cpp_server/cpp_server/src/Client/Client.cpp > CMakeFiles/cpp_server.dir/src/Client/Client.cpp.i

cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpp_server.dir/src/Client/Client.cpp.s"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/string/c/c/new_cpp_server/cpp_server/src/Client/Client.cpp -o CMakeFiles/cpp_server.dir/src/Client/Client.cpp.s

cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o: cpp_server/CMakeFiles/cpp_server.dir/flags.make
cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o: ../cpp_server/src/Client/HttpClient.cpp
cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o: cpp_server/CMakeFiles/cpp_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/string/c/c/new_cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o -MF CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o.d -o CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o -c /home/string/c/c/new_cpp_server/cpp_server/src/Client/HttpClient.cpp

cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.i"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/string/c/c/new_cpp_server/cpp_server/src/Client/HttpClient.cpp > CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.i

cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.s"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/string/c/c/new_cpp_server/cpp_server/src/Client/HttpClient.cpp -o CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.s

cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.o: cpp_server/CMakeFiles/cpp_server.dir/flags.make
cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.o: ../cpp_server/src/Server.cpp
cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.o: cpp_server/CMakeFiles/cpp_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/string/c/c/new_cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.o"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.o -MF CMakeFiles/cpp_server.dir/src/Server.cpp.o.d -o CMakeFiles/cpp_server.dir/src/Server.cpp.o -c /home/string/c/c/new_cpp_server/cpp_server/src/Server.cpp

cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cpp_server.dir/src/Server.cpp.i"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/string/c/c/new_cpp_server/cpp_server/src/Server.cpp > CMakeFiles/cpp_server.dir/src/Server.cpp.i

cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cpp_server.dir/src/Server.cpp.s"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/string/c/c/new_cpp_server/cpp_server/src/Server.cpp -o CMakeFiles/cpp_server.dir/src/Server.cpp.s

# Object files for target cpp_server
cpp_server_OBJECTS = \
"CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o" \
"CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o" \
"CMakeFiles/cpp_server.dir/src/Server.cpp.o"

# External object files for target cpp_server
cpp_server_EXTERNAL_OBJECTS =

cpp_server/libcpp_server.a: cpp_server/CMakeFiles/cpp_server.dir/src/Client/Client.cpp.o
cpp_server/libcpp_server.a: cpp_server/CMakeFiles/cpp_server.dir/src/Client/HttpClient.cpp.o
cpp_server/libcpp_server.a: cpp_server/CMakeFiles/cpp_server.dir/src/Server.cpp.o
cpp_server/libcpp_server.a: cpp_server/CMakeFiles/cpp_server.dir/build.make
cpp_server/libcpp_server.a: cpp_server/CMakeFiles/cpp_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/string/c/c/new_cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libcpp_server.a"
	cd /home/string/c/c/new_cpp_server/build/cpp_server && $(CMAKE_COMMAND) -P CMakeFiles/cpp_server.dir/cmake_clean_target.cmake
	cd /home/string/c/c/new_cpp_server/build/cpp_server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpp_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cpp_server/CMakeFiles/cpp_server.dir/build: cpp_server/libcpp_server.a
.PHONY : cpp_server/CMakeFiles/cpp_server.dir/build

cpp_server/CMakeFiles/cpp_server.dir/clean:
	cd /home/string/c/c/new_cpp_server/build/cpp_server && $(CMAKE_COMMAND) -P CMakeFiles/cpp_server.dir/cmake_clean.cmake
.PHONY : cpp_server/CMakeFiles/cpp_server.dir/clean

cpp_server/CMakeFiles/cpp_server.dir/depend:
	cd /home/string/c/c/new_cpp_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/string/c/c/new_cpp_server /home/string/c/c/new_cpp_server/cpp_server /home/string/c/c/new_cpp_server/build /home/string/c/c/new_cpp_server/build/cpp_server /home/string/c/c/new_cpp_server/build/cpp_server/CMakeFiles/cpp_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cpp_server/CMakeFiles/cpp_server.dir/depend
