#pragma once

#include "Edge.h"
class QuadEdge
{
public:
    QuadEdge();
    Edge edges[4];

    friend Edge* makeEdge(Vertex* a, Vertex* b);

    Edge* getEdge();

    //Edge* rot();
    //Edge* invRot();
    //Edge* sym();
    //Edge* oNext();
    //Edge* oPrev();
    //Edge* dNext();
    //Edge* dPrev();
    //Edge* lNext();
    //Edge* lPrev();
    //Edge* rNext();
    //Edge* rPrev();

    // Overloaded << operator as a friend function
    friend std::ostream& operator<<(std::ostream& os, const QuadEdge& edge);
};
