

	# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -g -O0 -Wall

# Directories
SRCDIR = .
BUILDDIR = build
TARGET = TriangulateInsertion

# Files and dependencies
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# Build rules
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean rule
clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: clean
