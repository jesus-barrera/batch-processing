CC = g++

OBJS = src/*.cpp src/ui/*.cpp src/ui/scheduler/*.cpp

INCLUDE_PATHS = -Iinclude

LINKER_FLAGS = -lncurses

COMPILER_FLAGS = -std=c++11 -Wall

scheduler:
	$(CC) $(OBJS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) -o bin/$@
