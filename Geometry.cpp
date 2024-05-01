#include "Geometry.h"
#include "common.h"

std::ostream &operator<<(std::ostream &os, const Vertex *edge)
{
    if (edge == nullptr)
    {
        os << " (vertex is null)";
    }
    else
    {

        os << "(" << edge->x << "," << edge->y << ")";
    }
    return os;
}
std::ostream &operator<<(std::ostream &os, const QuadEdge &qe)
{
    os << "QE{";
    for (int i = 0; i < 4; i++)
    {
        os << "\n\t" << &qe.edges[i];
    }
    os << "}";
    ;
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
Triangulation::Triangulation(Vertex *a, Vertex *b, Vertex *c)
{
    LOG(INFO) << "Creating triangulation";
    Edge *e1 = makeEdge();
    e1->setOrigin(a);
    e1->setDest(b);

    Edge *e2 = makeEdge();
    e2->setOrigin(b);
    e2->setDest(c);

    splice(e1->sym(), e2);

    Edge *e3 = makeEdge();
    e3->setOrigin(c);
    e3->setDest(a);
    splice(e2->sym(), e3);
    splice(e3->sym(), e1);
    LOG(INFO) << "Edge one: " << e1;
    LOG(INFO) << "Edge two: " << e2;
    LOG(INFO) << "Edge the: " << e3;

    this->startingEdge = e1;
}

void Triangulation::insertSite(Vertex *x)
{
    Edge *e = locate(x, startingEdge);
    LOG(INFO) << "located point" << e;

    if (x == e->getOrigin() || x == e->getDest())
    {
        // X is origin or destination of edge -> ignore + return
        LOG(INFO) << "point was org or dest";
        return;
    }
    else if (e->hasPoint(x))
    {
        // X lies on edge e
        LOG(INFO) << "point on edge";
        e = e->oPrev();
        deleteEdge(e->oNext());
    }

    Edge *base = makeEdge();
    LOG(INFO) << "Made QE";

    base->setOrigin(e->getOrigin());
    base->setDest(e->getDest());

    splice(base, e);
    startingEdge = base;

    LOG(INFO) << "Made base";

    do
    {
        base = connect(e, base->sym());
        e = base->oPrev();

    } while (e->lNext() != startingEdge);

    while (true)
    {
        Edge *t = e->oPrev();
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
