# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/bs08/e2309599/Documents/pielang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bs08/e2309599/Documents/pielang

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bs08/e2309599/Documents/pielang/CMakeFiles /home/bs08/e2309599/Documents/pielang/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/bs08/e2309599/Documents/pielang/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named PieLang

# Build rule for target.
PieLang: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 PieLang
.PHONY : PieLang

# fast build rule for target.
PieLang/fast:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/build
.PHONY : PieLang/fast

expression.o: expression.cpp.o

.PHONY : expression.o

# target to build an object file
expression.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/expression.cpp.o
.PHONY : expression.cpp.o

expression.i: expression.cpp.i

.PHONY : expression.i

# target to preprocess a source file
expression.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/expression.cpp.i
.PHONY : expression.cpp.i

expression.s: expression.cpp.s

.PHONY : expression.s

# target to generate assembly for a file
expression.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/expression.cpp.s
.PHONY : expression.cpp.s

function.o: function.cpp.o

.PHONY : function.o

# target to build an object file
function.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/function.cpp.o
.PHONY : function.cpp.o

function.i: function.cpp.i

.PHONY : function.i

# target to preprocess a source file
function.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/function.cpp.i
.PHONY : function.cpp.i

function.s: function.cpp.s

.PHONY : function.s

# target to generate assembly for a file
function.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/function.cpp.s
.PHONY : function.cpp.s

logger.o: logger.cpp.o

.PHONY : logger.o

# target to build an object file
logger.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/logger.cpp.o
.PHONY : logger.cpp.o

logger.i: logger.cpp.i

.PHONY : logger.i

# target to preprocess a source file
logger.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/logger.cpp.i
.PHONY : logger.cpp.i

logger.s: logger.cpp.s

.PHONY : logger.s

# target to generate assembly for a file
logger.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/logger.cpp.s
.PHONY : logger.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/main.cpp.s
.PHONY : main.cpp.s

object.o: object.cpp.o

.PHONY : object.o

# target to build an object file
object.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/object.cpp.o
.PHONY : object.cpp.o

object.i: object.cpp.i

.PHONY : object.i

# target to preprocess a source file
object.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/object.cpp.i
.PHONY : object.cpp.i

object.s: object.cpp.s

.PHONY : object.s

# target to generate assembly for a file
object.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/object.cpp.s
.PHONY : object.cpp.s

operation.o: operation.cpp.o

.PHONY : operation.o

# target to build an object file
operation.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/operation.cpp.o
.PHONY : operation.cpp.o

operation.i: operation.cpp.i

.PHONY : operation.i

# target to preprocess a source file
operation.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/operation.cpp.i
.PHONY : operation.cpp.i

operation.s: operation.cpp.s

.PHONY : operation.s

# target to generate assembly for a file
operation.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/operation.cpp.s
.PHONY : operation.cpp.s

operator.o: operator.cpp.o

.PHONY : operator.o

# target to build an object file
operator.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/operator.cpp.o
.PHONY : operator.cpp.o

operator.i: operator.cpp.i

.PHONY : operator.i

# target to preprocess a source file
operator.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/operator.cpp.i
.PHONY : operator.cpp.i

operator.s: operator.cpp.s

.PHONY : operator.s

# target to generate assembly for a file
operator.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/operator.cpp.s
.PHONY : operator.cpp.s

options.o: options.cpp.o

.PHONY : options.o

# target to build an object file
options.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/options.cpp.o
.PHONY : options.cpp.o

options.i: options.cpp.i

.PHONY : options.i

# target to preprocess a source file
options.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/options.cpp.i
.PHONY : options.cpp.i

options.s: options.cpp.s

.PHONY : options.s

# target to generate assembly for a file
options.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/options.cpp.s
.PHONY : options.cpp.s

scope.o: scope.cpp.o

.PHONY : scope.o

# target to build an object file
scope.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/scope.cpp.o
.PHONY : scope.cpp.o

scope.i: scope.cpp.i

.PHONY : scope.i

# target to preprocess a source file
scope.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/scope.cpp.i
.PHONY : scope.cpp.i

scope.s: scope.cpp.s

.PHONY : scope.s

# target to generate assembly for a file
scope.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/scope.cpp.s
.PHONY : scope.cpp.s

shell.o: shell.cpp.o

.PHONY : shell.o

# target to build an object file
shell.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/shell.cpp.o
.PHONY : shell.cpp.o

shell.i: shell.cpp.i

.PHONY : shell.i

# target to preprocess a source file
shell.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/shell.cpp.i
.PHONY : shell.cpp.i

shell.s: shell.cpp.s

.PHONY : shell.s

# target to generate assembly for a file
shell.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/shell.cpp.s
.PHONY : shell.cpp.s

stringliteral.o: stringliteral.cpp.o

.PHONY : stringliteral.o

# target to build an object file
stringliteral.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/stringliteral.cpp.o
.PHONY : stringliteral.cpp.o

stringliteral.i: stringliteral.cpp.i

.PHONY : stringliteral.i

# target to preprocess a source file
stringliteral.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/stringliteral.cpp.i
.PHONY : stringliteral.cpp.i

stringliteral.s: stringliteral.cpp.s

.PHONY : stringliteral.s

# target to generate assembly for a file
stringliteral.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/stringliteral.cpp.s
.PHONY : stringliteral.cpp.s

stringutils.o: stringutils.cpp.o

.PHONY : stringutils.o

# target to build an object file
stringutils.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/stringutils.cpp.o
.PHONY : stringutils.cpp.o

stringutils.i: stringutils.cpp.i

.PHONY : stringutils.i

# target to preprocess a source file
stringutils.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/stringutils.cpp.i
.PHONY : stringutils.cpp.i

stringutils.s: stringutils.cpp.s

.PHONY : stringutils.s

# target to generate assembly for a file
stringutils.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/stringutils.cpp.s
.PHONY : stringutils.cpp.s

system.o: system.cpp.o

.PHONY : system.o

# target to build an object file
system.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/system.cpp.o
.PHONY : system.cpp.o

system.i: system.cpp.i

.PHONY : system.i

# target to preprocess a source file
system.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/system.cpp.i
.PHONY : system.cpp.i

system.s: system.cpp.s

.PHONY : system.s

# target to generate assembly for a file
system.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/system.cpp.s
.PHONY : system.cpp.s

systemfunction.o: systemfunction.cpp.o

.PHONY : systemfunction.o

# target to build an object file
systemfunction.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/systemfunction.cpp.o
.PHONY : systemfunction.cpp.o

systemfunction.i: systemfunction.cpp.i

.PHONY : systemfunction.i

# target to preprocess a source file
systemfunction.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/systemfunction.cpp.i
.PHONY : systemfunction.cpp.i

systemfunction.s: systemfunction.cpp.s

.PHONY : systemfunction.s

# target to generate assembly for a file
systemfunction.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/systemfunction.cpp.s
.PHONY : systemfunction.cpp.s

userfunction.o: userfunction.cpp.o

.PHONY : userfunction.o

# target to build an object file
userfunction.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/userfunction.cpp.o
.PHONY : userfunction.cpp.o

userfunction.i: userfunction.cpp.i

.PHONY : userfunction.i

# target to preprocess a source file
userfunction.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/userfunction.cpp.i
.PHONY : userfunction.cpp.i

userfunction.s: userfunction.cpp.s

.PHONY : userfunction.s

# target to generate assembly for a file
userfunction.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/userfunction.cpp.s
.PHONY : userfunction.cpp.s

utils.o: utils.cpp.o

.PHONY : utils.o

# target to build an object file
utils.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/utils.cpp.o
.PHONY : utils.cpp.o

utils.i: utils.cpp.i

.PHONY : utils.i

# target to preprocess a source file
utils.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/utils.cpp.i
.PHONY : utils.cpp.i

utils.s: utils.cpp.s

.PHONY : utils.s

# target to generate assembly for a file
utils.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/utils.cpp.s
.PHONY : utils.cpp.s

value.o: value.cpp.o

.PHONY : value.o

# target to build an object file
value.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/value.cpp.o
.PHONY : value.cpp.o

value.i: value.cpp.i

.PHONY : value.i

# target to preprocess a source file
value.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/value.cpp.i
.PHONY : value.cpp.i

value.s: value.cpp.s

.PHONY : value.s

# target to generate assembly for a file
value.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/value.cpp.s
.PHONY : value.cpp.s

variable.o: variable.cpp.o

.PHONY : variable.o

# target to build an object file
variable.cpp.o:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/variable.cpp.o
.PHONY : variable.cpp.o

variable.i: variable.cpp.i

.PHONY : variable.i

# target to preprocess a source file
variable.cpp.i:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/variable.cpp.i
.PHONY : variable.cpp.i

variable.s: variable.cpp.s

.PHONY : variable.s

# target to generate assembly for a file
variable.cpp.s:
	$(MAKE) -f CMakeFiles/PieLang.dir/build.make CMakeFiles/PieLang.dir/variable.cpp.s
.PHONY : variable.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... PieLang"
	@echo "... edit_cache"
	@echo "... expression.o"
	@echo "... expression.i"
	@echo "... expression.s"
	@echo "... function.o"
	@echo "... function.i"
	@echo "... function.s"
	@echo "... logger.o"
	@echo "... logger.i"
	@echo "... logger.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... object.o"
	@echo "... object.i"
	@echo "... object.s"
	@echo "... operation.o"
	@echo "... operation.i"
	@echo "... operation.s"
	@echo "... operator.o"
	@echo "... operator.i"
	@echo "... operator.s"
	@echo "... options.o"
	@echo "... options.i"
	@echo "... options.s"
	@echo "... scope.o"
	@echo "... scope.i"
	@echo "... scope.s"
	@echo "... shell.o"
	@echo "... shell.i"
	@echo "... shell.s"
	@echo "... stringliteral.o"
	@echo "... stringliteral.i"
	@echo "... stringliteral.s"
	@echo "... stringutils.o"
	@echo "... stringutils.i"
	@echo "... stringutils.s"
	@echo "... system.o"
	@echo "... system.i"
	@echo "... system.s"
	@echo "... systemfunction.o"
	@echo "... systemfunction.i"
	@echo "... systemfunction.s"
	@echo "... userfunction.o"
	@echo "... userfunction.i"
	@echo "... userfunction.s"
	@echo "... utils.o"
	@echo "... utils.i"
	@echo "... utils.s"
	@echo "... value.o"
	@echo "... value.i"
	@echo "... value.s"
	@echo "... variable.o"
	@echo "... variable.i"
	@echo "... variable.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

