
#include "easylogging++.h"

#include "Triangulation.h"
#include "common.h"

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
