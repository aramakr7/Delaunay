

#include "Edge.h"
#include "Vertex.h"
#include "easylogging++.h"




bool operator==(const Vertex& l, const Vertex& r)
{
    return (l.x == r.x && l.y == r.y);
}
bool operator==(const Edge& l, const Edge& r)
{
    return (l.origin == r.origin && l.dest == r.dest) || (l.origin == r.dest && l.dest == r.origin);
}

bool operator<(const Vertex& l, const Vertex& r)
{
    return (l.x < r.x && l.y < r.y);
}
bool operator<(const Edge& l, const Edge& r)
{
    return l.origin < r.origin;
}

Edge::Edge(Vertex* origin)
{
    this->origin = origin;
}

Edge::Edge()
    : origin(new Vertex(-1, -1))
    , dest(new Vertex(-1, -1))
{
}

Edge::~Edge()
{
}

Edge *Edge::rot()
{
    LOG(DEBUG);
    if (this->index == 3)
    {
        return this - 3;
    }

    return this + 1;
}

Edge *Edge::invRot()
{
    LOG(DEBUG);
    if (this->index == 0)
    {
        return this + 3;
    }

    return this - 1;
}

Edge *Edge::sym()
{
    LOG(DEBUG);
    if (this->index >= 2)
    {
        return this - 2;
    }

    return this + 2;
}

Edge* Edge::oNext()
{
    LOG(DEBUG);
    return this->next;
}

Edge* Edge::oPrev()
{
    LOG(DEBUG);
    return this->rot()->oNext()->rot();
}

Edge* Edge::dNext()
{
    LOG(DEBUG);
    return this->sym()->oNext()->sym();
}

Edge* Edge::dPrev()
{
    LOG(DEBUG);
    return this->invRot()->oNext()->invRot();
}

Edge* Edge::lNext()
{
    LOG(DEBUG);
    return this->invRot()->oNext()->rot();
}

Edge* Edge::lPrev()
{
    LOG(DEBUG);
    return this->oNext()->sym();
}

Edge* Edge::rNext()
{
    LOG(DEBUG);
    return this->rot()->oNext()->invRot();
}

Edge* Edge::rPrev()
{
    LOG(DEBUG);
    return this->sym()->oNext();
}

void Edge::setNext(Edge* next)
{
    this->next = next;
}

void Edge::setOrigin(Vertex* origin)
{
    LOG(INFO) << "setOrigin" << origin;
    this->origin = origin;
    (this + ((2+index)%4))->dest = origin;
}

Vertex* Edge::getOrigin()
{
    return this->origin;
}

void Edge::setDest(Vertex* dest)
{
    this->dest = dest;
    (this + ((2+index)%4))->origin = dest;
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
    return ((this->dest->y - this->origin->y) / (this->dest->x - this->origin->x));
}

bool Edge::hasPoint(Vertex* p)
{

    LOG(INFO) << "In has point";

    if ((p->x < this->origin->x && p->x < this->dest->x) || (p->x > this->origin->x && p->x > this->dest->x) ||
        (p->y < this->origin->y && p->y < this->dest->y) || (p->y > this->origin->y && p->y > this->dest->y))
    {
        LOG(INFO) << "Here?";

        return false;
    }
    else
    {
        LOG(INFO) << "Here?";
        float pointSlope = ((this->dest->y - p->y) / (this->dest->x - p->x));
        LOG(INFO) << "Here?";
        if (abs(pointSlope) == abs(this->slope()))
        {
            return true;
        }
    }

    LOG(INFO) << "Here?";
    return false;
}
// Definition of the overloaded << operator
std::ostream& operator<<(std::ostream& os, const Edge* edge)
{
    //os << "Edge: ([" << edge->index << "] ";
    //os << "Origin:" << edge->origin;
    //os << " Dest:" << edge->dest;
    //os << ")";
    os << edge->origin << "," << edge->dest;
    return os;
}
