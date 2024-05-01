#include "Geometry.h"
#include "common.h"

Edge::Edge(Vertex* origin)
{
    this->origin = origin;
}

Edge::Edge(){}
Edge::~Edge(){}

Edge* Edge::rot()
{
    LOG(DEBUG);
    if (this->index == 3)
    {
        return this - 3;
    }

    return this + 1;

}

Edge* Edge::invRot()
{
    LOG(DEBUG);
    if (this->index == 0)
    {
        return this + 3;
    }

    return this - 1;

}

Edge* Edge::sym()
{
    LOG(DEBUG);
    if (this->index > 2)
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

    LOG(INFO) << "In has point" ;

    if ((p->x < this->origin->x && p->x < this->dest->x)
        || (p->x > this->origin->x && p->x > this->dest->x)
        || (p->y < this->origin->y && p->y < this->dest->y)
        || (p->y > this->origin->y && p->y > this->dest->y))
    {
        LOG(INFO) << "Here?" ;

        return false;
    }
    else
    {
        LOG(INFO) << "Here?" ;
        float pointSlope = ((this->dest->y - p->y)/(this->dest->x - p->x));
        LOG(INFO) << "Here?" ;
        if (abs(pointSlope) == abs(this->slope()))
        {
            return true;
        }
    }

    LOG(INFO) << "Here?" ;
    return false;

}
// Definition of the overloaded << operator
std::ostream& operator<<(std::ostream& os, const Edge* edge) {
    os << "Edge: (["<< edge->index << "] ";
    if ( edge->origin != nullptr )
    {
        os << "Origin:" << edge->origin;
    }
    else
    {
        os << "Origin: null";
    }
    if ( edge->dest != nullptr )
    {
        os <<" Dest:" << edge->dest;
    }
    else
    {
        os << "Dest: null";
    }
    os <<  ")";
    return os;
}


std::ostream& operator<<(std::ostream& os, const Vertex* edge) {
    if (edge == nullptr)
    {   
        os << " (vertex is null)";

    }
    else{

        os << "(" << edge->x << ","  << edge->y << ")";
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
    return((this->x * this->x) + (this->y * this->y));
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

// Creates a triangle given 3 vertices
Triangulation::Triangulation(Vertex* a, Vertex* b, Vertex* c)
{
    LOG(INFO) << "Creating triangulation";
    Edge *e1 = makeEdge();
    e1->setOrigin(a);
    e1->setDest(b);

    Edge *e2 = makeEdge();
    e2->setOrigin(b);
    e2->setDest(c);

    splice(e1->sym(), e2);

    Edge* e3 = makeEdge();
    e3->setOrigin(c);
    e3->setDest(a);
    splice(e2->sym(), e3);
    splice(e3->sym(), e1);
    LOG(INFO) << "Edge one: " << e1;
    LOG(INFO) << "Edge two: " << e2;
    LOG(INFO) << "Edge the: " << e3;

    this->startingEdge = e1;

}


void Triangulation::insertSite(Vertex* x)
{
    Edge *e = locate(x, startingEdge);
    LOG(INFO) << "located point" << e;

    if (x == e->getOrigin() || x == e->getDest())
    {
        // X is origin or destination of edge -> ignore + return
        LOG(INFO) << "point was org or dest" ;
        return;
    }
    else if (e->hasPoint(x))
    {
        // X lies on edge e
        LOG(INFO) << "point on edge" ;
        e = e->oPrev();
        deleteEdge(e->oNext());
    }

    Edge *base = makeEdge();
    LOG(INFO) << "Made QE" ;

    base->setOrigin(e->getOrigin());
    base->setDest(e->getDest());

    splice(base, e);
    startingEdge = base;

     LOG(INFO) << "Made base" ;


    do
    {
        base = connect(e, base->sym());
        e = base->oPrev();

    } while (e->lNext() != startingEdge);

    while (true)
    {
        Edge* t = e->oPrev();
        if (RightOf(e, t->getDest()) &&
            InCircle(e->getOrigin(), t->getDest(), e->getDest(), x))
            {
                swapEdge(e);
                e = e->oPrev();
            }
        else if (e->oNext() == startingEdge)
        {
            return;
        }
        else
        {
            e = e->oNext()->lPrev();
        }

    }
}

