#include "Vertex.h"
#include "easylogging++.h"
std::ostream &operator<<(std::ostream &os, const Vertex *edge)
{
    if (edge == nullptr)
    {
        os << " (vertex is null)";
    }
    else
    {

        os << edge->x << "," << edge->y;
    }
    return os;
}

Vertex::Vertex(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vertex::lengthSquared()
{
    return ((this->x * this->x) + (this->y * this->y));
}
