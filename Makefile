CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

# Target executables
TARGETS = firstfit bestfit

# Source files
SOURCES = main.cpp allocator.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Make all targets
all: $(TARGETS)

# Rule for firstfit
firstfit: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Rule for bestfit
bestfit: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

# Clean up object and executable files
clean:
	rm -f $(OBJECTS) $(TARGETS)
