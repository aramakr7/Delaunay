PROGRAM = TriangulateInsertion

SOURCES = \
	Geometry.cpp \
	TriangulateInsertion.cpp \
	determinate.cpp \
	easylogging++.cc \
	$()

$(PROGRAM): $(SOURCES)
	g++ -std=c++11 -g -O0 -o $(PROGRAM) $(SOURCES)

clean:
	rm -f $(PROGRAM)


	# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++14

# Directories
SRCDIR = .
BUILDDIR = build
TARGET = myprogram

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
