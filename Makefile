# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Werror -std=c++14

# Source files
SRC = main.cpp allocator.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Executables
EXEC_FIRSTFIT = firstfit
EXEC_BESTFIT = bestfit

# Default target
all: $(EXEC_FIRSTFIT) $(EXEC_BESTFIT)

# Build firstfit
$(EXEC_FIRSTFIT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build bestfit
$(EXEC_BESTFIT): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean up object files and executables
clean:
	rm -f $(OBJ) $(EXEC_FIRSTFIT) $(EXEC_BESTFIT)

# Phony targets
.PHONY: all clean
