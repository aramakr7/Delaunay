#include "QuadEdge.h"

std::ostream& operator<<(std::ostream& os, const QuadEdge& qe)
{
    os << "\nQE{";
    for (int i = 0; i < 4; i++)
    {
        os << "\n\t" << &qe.edges[i];
    }
    os << "\n}";
    ;
    return os;
}

QuadEdge::QuadEdge()
{
    edges[0].setIndex(0);
    edges[0].setNext((edges + 0));

    edges[1].setIndex(1);
    edges[1].setNext((edges + 3));

    edges[2].setIndex(2);
    edges[2].setNext((edges + 2));

    edges[3].setIndex(3);
    edges[3].setNext((edges + 1));
}