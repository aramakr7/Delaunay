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