#pragma once
#include "Vertex.h"
#include "Edge.h"

class Triangulation
{
public:
    Triangulation(Vertex *a, Vertex *b, Vertex *c);
    void insertSite(Vertex *x);

private:
    Edge *startingEdge;
};
