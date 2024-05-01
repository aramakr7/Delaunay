#pragma once

#include "Edge.h"
class QuadEdge
{
    public:
        QuadEdge();
        Edge edges[4];

    friend Edge* makeEdge(Vertex *a, Vertex *b);
        // Overloaded << operator as a friend function
        friend std::ostream& operator<<(std::ostream& os, const QuadEdge& edge);
};
