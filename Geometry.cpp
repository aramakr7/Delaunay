#include "Geometry.h"

Edge::Edge(Vertex* origin)
{
    this->origin = origin;
}

Edge::Edge(){}
Edge::~Edge(){}

Edge* Edge::rot()
{
    if (this->index == 3)
    {
        return this - 3;
    }

    return this + 1;

}

Edge* Edge::invRot()
{
    if (this->index == 0)
    {
        return this + 3;
    }

    return this - 1;

}

Edge* Edge::sym()
{
    if (this->index > 2)
    {
        return this - 2;
    }

    return this + 2;
}

Edge* Edge::oNext()
{
    return this->next;
}

Edge* Edge::oPrev()
{
    return this->rot()->oNext()->rot();
}

Edge* Edge::dNext()
{
    return this->sym()->oNext()->sym();
}

Edge* Edge::dPrev()
{
    return this->invRot()->oNext()->invRot();
}

Edge* Edge::lNext()
{
    return this->invRot()->oNext()->rot();
}

Edge* Edge::lPrev()
{
    return this->oNext()->sym();
}

Edge* Edge::rNext()
{
    return this->rot()->oNext()->invRot();
}

Edge* Edge::rPrev()
{
    return this->sym()->oNext();
}

void Edge::setNext(Edge* next)
{
    this->next = next;
}

void Edge::setOrigin(Vertex* origin)
{
    this->origin = origin;
}

Vertex* Edge::getOrigin()
{
    return this->origin;
}

void Edge::setDest(Vertex* dest)
{
    this->dest = dest;
}

Vertex* Edge::getDest()
{
    return this->dest;
}

void Edge::setIndex(int index)
{
    this->index = index;
}

int Edge::getIndex()
{
    return this->index;
}


float Edge::slope()
{
    return ((this->dest->y - this->origin->y)/(this->dest->x- this->origin->x));
}

bool Edge::hasPoint(Vertex* p)
{
    if ((p->x < this->origin->x && p->x < this->dest->x)
        || (p->x > this->origin->x && p->x > this->dest->x)
        || (p->y < this->origin->y && p->y < this->dest->y)
        || (p->y > this->origin->y && p->y > this->dest->y))
    {
        return false;
    }
    else
    {
        float pointSlope = ((this->dest->y - p->y)/(this->dest->x - p->x));
        if (abs(pointSlope) == abs(this->slope()))
        {
            return true;
        } 
    }

    return false;

}


Vertex::Vertex(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vertex::lengthSquared()
{
    return((this->x * this->x) + (this->y * this->y));
}



QuadEdge::QuadEdge()
{
    edges[0].setIndex(0);
    edges[0].setNext((edges + 0));

    edges[0].setIndex(0);
    edges[1].setNext((edges + 3));

    edges[2].setIndex(2);
    edges[2].setNext((edges + 2));

    edges[3].setIndex(3);
    edges[3].setNext((edges + 1));
}

