PROGRAM = TriangulateInsertion

SOURCES = \
	Geometry.cpp \
	TriangulateInsertion.cpp \
	$()

$(PROGRAM): $(SOURCES)
	g++ -std=c++11 -o $(PROGRAM) $(SOURCES)

clean:
	rm -f $(PROGRAM)